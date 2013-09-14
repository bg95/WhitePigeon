#include "WPSingingVoiceAnalyser.h"

WPSingingVoiceAnalyser::WPSingingVoiceAnalyser()
{
	Ob.clear();
	Ob[0] = 0;
	Ob[20000] = 0;
}

WPSingingVoiceAnalyser::~WPSingingVoiceAnalyser()
{
}

void WPSingingVoiceAnalyser::__test()
{
	W.readFile("/home/token/Downloads/build-WhitePigeon-Qt_5_0_1_System-Debug/wave a.wav");
	qDebug("I'm Pt");
	connect(&W, SIGNAL(finished()), this, SLOT(__test2()));
	qDebug("I'm Cr");
}

void WPSingingVoiceAnalyser::__test2()
{
	int K = W.getData().size();
	qDebug("K = %d", K);
	std::complex <double> *Height = new std::complex <double>[K];
	for (int i = 0; i < K; ++ i)
	{
		Height[i] = W.data[i];
	}
	int M = 1;
	while (M * 2 <= K)
		M *= 2;
	QVector < std::complex <double> > Result;
	int N = WPWave::FFT(Height + 1000, Height + M + 1000, Result);
	delete[] Height;
	FILE *fo = fopen("a.txt", "w");
	for (int k = 0; k < (int) Result.size(); ++ k)
		if (k - 2 < 0 || k + 2 >= (int) Result.size() || (std::abs(Result[k - 2]) < std::abs(Result[k]) && std::abs(Result[k]) > std::abs(Result[k + 2])))
			fprintf(fo, "%d %.3lf %.3lf\n", k, (double) k / ((double) M / (double) WPWave::SamplingRate), std::abs(Result[k]));
	fclose(fo);
	std::vector < std::pair <double, double> > Candidates;
	for (int i = 0; i < 1000; ++ i)
	{
		double x = 171.0 + (double) (rand() % 10000) / 9999 * 3.0;
		double F = 0;
		for (int j = 1, k = 1; x * k < 20000 && x * k < std::abs(Result.back()); ++ k)
			for (; j < (int) Result.size(); ++ j)
				if ((double) (j - 1) / ((double) M / (double) WPWave::SamplingRate) <= x * k && x * k < (double) j / ((double) M / (double) WPWave::SamplingRate))
				{
					F += pow((std::abs(Result[j - 1]) * ((double) j / ((double) M / (double) WPWave::SamplingRate) - x * k) + std::abs(Result[j]) * (x * k - (double) (j - 1) / ((double) M / (double) WPWave::SamplingRate))) / (1.0 / ((double) M / (double) WPWave::SamplingRate)), 2);
					break;
				}
		Candidates.push_back(std::make_pair(F, x));
	}
	for (int t = 0; t < 10; ++ t)
	{
		std::sort(Candidates.begin(), Candidates.end());
		std::reverse(Candidates.begin(), Candidates.end());
		Candidates.resize(100);
		for (int i = 0; i < 200; ++ i)
		{
			int a = rand() % 100;
			int b = rand() % 100;
			double x = (Candidates[a].second + Candidates[b].second) / 2;
			if (rand() % 3 == 0)
			{
				int u = rand() % 999 + 1;
				int v = rand() % 999 + 1;
				x = (Candidates[a].second * u + Candidates[b].second * v) / (u + v);
			}
			double F = 0;
			for (int j = 1, k = 1; x * k < 20000 && x * k < std::abs(Result.back()); ++ k)
				for (; j < (int) Result.size(); ++ j)
					if ((double) (j - 1) / ((double) M / (double) WPWave::SamplingRate) <= x * k && x * k < (double) j / ((double) M / (double) WPWave::SamplingRate))
					{
						F += pow((std::abs(Result[j - 1]) * ((double) j / ((double) M / (double) WPWave::SamplingRate) - x * k) + std::abs(Result[j]) * (x * k - (double) (j - 1) / ((double) M / (double) WPWave::SamplingRate))) / (1.0 / ((double) M / (double) WPWave::SamplingRate)), 2);
						break;
					}
			Candidates.push_back(std::make_pair(F, x));
		}
	}
	std::sort(Candidates.begin(), Candidates.end());
	fo = fopen("h.txt", "w");
	fprintf(fo, "%.3lf\n", Candidates.back().second);
	fclose(fo);
	double BaseFreq = Candidates.back().second;
	for (int j = 1, k = 1; BaseFreq * k < 20000 && BaseFreq * k < std::abs(Result.back()); ++ k)
		for (; j < (int) Result.size(); ++ j)
			if ((double) (j - 1) / ((double) M / (double) WPWave::SamplingRate) <= BaseFreq * k && BaseFreq * k < (double) j / ((double) M / (double) WPWave::SamplingRate))
			{
				Ob[BaseFreq * k] = (std::abs(Result[j - 1]) * ((double) j / ((double) M / (double) WPWave::SamplingRate) - BaseFreq * k) + std::abs(Result[j]) * (BaseFreq * k - (double) (j - 1) / ((double) M / (double) WPWave::SamplingRate))) / (1.0 / ((double) M / (double) WPWave::SamplingRate));
				break;
			}
	qDebug("finish __test2()");
	__test3();
	/*
	double L = 171, R = 174;
	while (R - L > 1e-3)
	{
		double M1 = (L * 2 + R) / 3;
		double M2 = (L + R * 2) / 3;
		double F1 = 0;
		double F2 = 0;
		for (int k = 1; M2 * k < 20000 && M2 * k < std::abs(Result.back()); ++ k)
		{
			for (int j = 1; j < (int) Result.size(); ++ j)
			{
				if (std::abs(Result[j - 1]) <= M1 * k && M1 * k < std::abs(Result[j]))
					F1 += pow((std::abs(Result[j - 1]) * (std::abs(Result[j]) - M1 * k) + std::abs(Result[j]) * (M1 * k - std::abs(Result[j - 1]))) / (std::abs(Result[j]) - std::abs(Result[j - 1])), 2);
				if (std::abs(Result[j - 1]) <= M2 * k && M2 * k < std::abs(Result[j]))
					F2 += pow((std::abs(Result[j - 1]) * (std::abs(Result[j]) - M2 * k) + std::abs(Result[j]) * (M2 * k - std::abs(Result[j - 1]))) / (std::abs(Result[j]) - std::abs(Result[j - 1])), 2);
			}
		}
		qDebug("M1: %lf %lf :: M2: %lf %lf", M1, F1, M2, F2);
		if (F1 > F2)
			R = M2;
		else
			L = M1;
	}
	 */
}

void WPSingingVoiceAnalyser::__test3()
{
	double f = 221.0;
	std::vector <long long> R;
	double Max = 1.0;
	for (int i = 0; i < 99999; ++ i)
	{
		double t = 1.0 / WPWave::SamplingRate * i;
		double h = 0;
		for (int k = 1; f * k < 20000; ++ k)
		{
			h += Fun(f * k) * sin(WPWave::PI * 2 * f * k * t);
			//qDebug("k = %d, f = %lf, func = %lf", k, f * k, Fun(f * k));
		}
		R.push_back(h);
		checkmax(Max, fabs(h));
	}
	QVector <WPWave::WaveDataType> D;
	for (int i = 0; i < (int) R.size(); ++ i)
		D.push_back(R[i] / Max * 30000);
	W.setData(D);
	W.play();
}

double WPSingingVoiceAnalyser::Fun(double X)
{
	std::map <double, double> :: iterator it2 = Ob.upper_bound(X);
	if (it2 == Ob.begin())
		return 0;
	std::map <double, double> :: iterator it1 = it2;
	-- it1;
	if (it2 == Ob.end())
		return 0;
	double Result = (it1->second * (it2->first - X) + it2->second * (X - it1->first)) / (it2->first - it1->first);
	return Result;
}
