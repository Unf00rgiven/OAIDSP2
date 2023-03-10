
#ifndef IMAGEPROCESSING_H_
#define IMAGEPROCESSING_H_

#include <QString>
#include <QVector>
#include <QImage>

void imageProcessingFun(
	const QString& progName,
	QImage& outImgs,
	const QImage& inImgs,
	const QVector<double>& params);

#endif // IMAGEPROCESSING_H_
