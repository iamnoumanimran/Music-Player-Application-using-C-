#pragma once
#include <QLabel>
#include <QDebug>
#include <vector>

class FastFourierTransform;

using namespace std;

class Spectrum : public QLabel {
	Q_OBJECT
	friend class MainWindow;

public:
	Spectrum(QWidget *parent = NULL);
	~Spectrum();
	
private:
	Spectrum(const Spectrum &);
	Spectrum& operator=(const Spectrum &);
	void paintEvent(QPaintEvent *event);
	FastFourierTransform *FFT;	
	short *sample;				
	double *FFT_result;			
	bool powerSpectrum(short * sampleData, int totalSamples,
		int channels, double *power);
};