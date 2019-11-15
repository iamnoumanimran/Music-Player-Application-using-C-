#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <cmath>
#include "spectrum.h"
#include "FFT.h"

static const int num = 32;
static const double pi = 3.1415926535898;

Spectrum::Spectrum(QWidget *parent):QLabel(parent)
{
	this->resize(400, 200);
	setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint); 
	FFT = new FastFourierTransform(num);
	FFT_result = new double[num];
	sample = new short[num];
	for (int i = 0; i < num; i++)
		FFT_result[i] = 0.0;
}

Spectrum::~Spectrum() 
{
	delete[] sample;
	delete[] FFT_result;
	delete FFT;
}



void Spectrum::paintEvent(QPaintEvent *event) {
	Q_UNUSED(event)
	QPainter painter(this);
	
	
	int barNum = num;
	if (barNum != 0) {

		int singleWidth = this->width() / barNum;
		int barWidth = 0.8 * singleWidth;
		int gapWidth = singleWidth - barWidth;
		int barHeight = this->height() - 2 * gapWidth;

		for (int i = 0; i < barNum; i++) {
			float value = FFT_result[i];
			QRect bar = rect();
			bar.setLeft(i * singleWidth);
			bar.setWidth(barWidth);
			bar.setTop(rect().top() + gapWidth +
				(1.0 - value) * barHeight);
			bar.setBottom(rect().bottom() - gapWidth);
			QColor barColor(7*i, 0.9 * i * (num - i), 7*(num - i));
			painter.fillRect(bar, barColor);
		}
	}
	event->accept();
}

bool Spectrum::powerSpectrum(short * sampleData, int totalSamples,int channels, double *power) {
	complex<float> * sample;

	for (int i = 0; i < totalSamples; i++)
		qDebug() << sampleData[i];

	if (channels == 1) {
		sample = new complex<float>[totalSamples];
		for (int i = 0; i < totalSamples; i++)
			sample[i] = complex<float>(sampleData[i] / 32768.0, 0);
	}
	else {
		sample = new complex<float>[totalSamples / 2];
		for (int i = 0; i < totalSamples; i += 2)
			sample[i/2] = complex<float>((sampleData[i]+ sampleData[i+1]) / 65536.0, 0);
	}
	int log2N = log2(totalSamples/channels - 1) + 1;	
	int sign = -1;

	FFT->process(sample, log2N, sign); 

	for (int i = 0; i < totalSamples/channels; i++) {
		power[i] = abs(sample[i]) / sqrt(2) / 2;
		if (power[i] < 0 || power[i] > 1)
			power[i] = 0;
	}
	delete [] sample;
	return true;
}