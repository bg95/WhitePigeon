/*
 * 2013-05-15  Token  <token@token-HP-ENVY-14-SPECTRE-Notebook-PC>

 * 
 * selfplayer
 * It is used to play the sound.
 * Get all the sound nearly and mix them together.
 */
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include "SDL/SDL_mixer.h"

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <map>

#include "WPScore.h"

#define pi 3.14159265358979323846264338327950288419716939937510
#define tr(i, x) for (typeof(x.begin()) i = x.begin(); i != x.end(); ++ i)
#define pii std::pair <int, int>

namespace Player
{
	#define BUFFER 1024
	
	void errorv(char *str, va_list ap)
	{
		vfprintf(stderr,str,ap);
		fprintf(stderr,": %s.\n", SDL_GetError());
	}
	
	void cleanExit(char *str, ...)
	{
		va_list ap;
		va_start(ap, str);
		errorv(str, ap);
		va_end(ap);
		Mix_CloseAudio();
		SDL_Quit();
		exit(1);
	}
	
	void AddStr(std::vector <int> &v, std::string s)
	{
		for (int i = 0; i < (int) s.size(); ++ i)
		{
			int t = (unsigned char) s[i];
			v.push_back(t);
		}
	}

	void AddInt(std::vector <int> &v, unsigned long long x, int w)
	{
		for (int k = 0; k < w; ++ k)
		{
			v.push_back(x & 255);
			x >>= 8;
		}
	}
	
	std::vector <int> Build(double T, double F)
	{
		std::vector <int> Result;
		AddStr(Result, "RIFF");
		AddInt(Result, 100040 - 8, 4); // Need Modifying
		AddStr(Result, "WAVE");
		AddStr(Result, "fmt ");
		AddInt(Result, 0x10, 4);
		AddInt(Result, 1, 2);
		AddInt(Result, 1, 2);
		AddInt(Result, 44100, 4);
		AddInt(Result, 44100, 4);
		AddInt(Result, 1, 2);
		AddInt(Result, 8, 2);
		AddStr(Result, "data");
		AddInt(Result, T * 44100, 4);
		for (int t = 0; t < T * 44100; ++ t)
		{
			double x = (double) t / 44100.0;
			double y = sin(2.0 * F * pi * x) + 0.01 * sin(2.0 * F / 2.0 * pi * x) + 0.01 * sin(2.0 * F / 3.0 * pi * x) + 0.05 * sin(2.0 * F / 4.0 * pi * x) + 0.2 * sin(2.0 * F / 5.0 * pi * x) + 0.3 * sin(2.0 * F * 2.0 * pi * x) + 0.2 * sin(2.0 * F * 3.0 * pi * x) + 0.2 * sin(2.0 * F * 4.0 * pi * x) + 0.2 * sin(2.0 * F * 5.0 * pi * x) + 0.15 * sin(2.0 * F * 6.0 * pi * x);
			AddInt(Result, (int) round((y + 3) * 50), 1);
		}
		int Size = Result.size() - 8;
		Result[4] = Size & 255;
		Result[5] = (Size >> 8) & 255;
		Result[6] = (Size >> 16) & 255;
		Result[7] = (Size >> 24) & 255;
		return Result;
	}
	
	void Write(FILE *fo, std::vector <int> v)
	{
		for (int k = 0; k < (int) v.size(); ++ k)
			fprintf(fo, "%c", (char) v[k]);
	}
	
	void PutPitch(double StartTime, double EndTime, double Freq)
	{
		FILE *fp = fopen("[Tmp]Pt-Cr.wav", "w");
		Write(fp, Build(EndTime - StartTime, Freq));
		fclose(fp);
	}
	
	void PutEmpty(double EndTime, double StartTime)
	{
		FILE *fp = fopen("[Tmp]Pt-Cr.wav", "w");
		Write(fp, Build(EndTime - StartTime, 0));
		fclose(fp);
	}
	
	void Run()
	{
		std::cout << "Please input your note:";
		int P, X, Y;
		WPScore WPS;
		int k = 1;
		while (std::cin >> P >> X >> Y)
		{
			
			WPNote N(P, Fraction (X, Y));
			WPS.insertNote(Fraction (k, 1), N);
			++ k;
		}
		
		std::vector < std::pair <WPPosition, WPNote> > NL = WPS.getNoteList();
		double LastTime = 0;
		reverse(NL.begin(), NL.end());
		
		/// Initialize SDL for audio and video
		if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
			cleanExit("SDL_Init");
		atexit(SDL_Quit);
		
		/// initialize sdl mixer, open up the audio device
		if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, BUFFER) < 0)
			cleanExit("Mix_OpenAudio");
		
		/// we play no samples, so deallocate the default 8 channels
		Mix_AllocateChannels(0);
		
		int volume = SDL_MIX_MAXVOLUME;
		bool done = 0;
		while (!NL.empty())
		{
			double StartTime = NL.back().first.getValue().toDouble();
			double EndTime = StartTime + NL.back().second.getTimeSpan() * 60.0 / 132.0;
			double Freq = NL.back().second.getFrequency();
			if (NL.back().second.getPitch() <= - 999999)
				Freq = 0;
			PutEmpty(LastTime, StartTime);
			PutPitch(StartTime, EndTime, Freq);
			LastTime = EndTime;
			NL.pop_back();
			Mix_Music *music;
			if (!(music = Mix_LoadMUS("[Tmp]Pt-Cr.wav")))
				cleanExit("Mix_LoadMUS(\"%s\")", "[Tmp]Pt-Cr.wav");
			if (Mix_PlayMusic(music, 1) == - 1)
				cleanExit("Mix_PlayMusic(0x%p,1)", music);
			while ((Mix_PlayingMusic() || Mix_PausedMusic()) && !done)
			{
				//~ SDL_Event e;
				//~ while(SDL_PollEvent(&e))
				//~ {
					//~ switch(e.type)
					//~ {
						//~ case SDL_QUIT:
							//~ done = 1;
							//~ break;
						//~ case SDL_KEYDOWN:
							//~ switch(e.key.keysym.sym)
							//~ {
								//~ case SDLK_ESCAPE:
									//~ done = 1;
									//~ break;
								//~ case SDLK_UP:
									//~ volume = (volume + 1) << 1;
									//~ if (volume > SDL_MIX_MAXVOLUME)
										//~ volume = SDL_MIX_MAXVOLUME;
									//~ Mix_VolumeMusic(volume);
									//~ break;
								//~ case SDLK_DOWN:
									//~ volume >>= 1;
									//~ Mix_VolumeMusic(volume);
									//~ break;
								//~ case SDLK_SPACE:
									//~ if (Mix_PausedMusic())
										//~ Mix_ResumeMusic();
									//~ else
										//~ Mix_PauseMusic();
									//~ break;
								//~ default:
									//~ break;
							//~ }
						//~ default:
							//~ break;
					//~ }
				//~ }
			}
			Mix_VolumeMusic(volume);
			Mix_FreeMusic(music);
		}
		
		/// free & close
		Mix_CloseAudio();
		SDL_Quit();
	}
	
	#undef BUFFER
}

int main(int argc, char **argv)
{
	Player::Run();
	return 0;
}
