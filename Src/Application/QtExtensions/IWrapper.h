#pragma once

#include <QtCore/QObject>

class IWrapper : public QObject
{
Q_OBJECT
public:
	IWrapper() {}
	virtual ~IWrapper() {}

public slots:
	virtual void slotStateChanged(int state) {}
	virtual void slotDValueChanged(double value) {}
	virtual void slotFValueChanged(double value) {}
	virtual void slotIValueChanged(int value) {}
	virtual void slotUValueChanged(int value) {}
	virtual void slotTextChanged(const QString &text) {}
	virtual void slot1ValueChanged(double value) {}
	virtual void slot2ValueChanged(double value) {}
	virtual void slot3ValueChanged(double value) {}
	virtual void slot4ValueChanged(double value) {}
	virtual void slot5ValueChanged(double value) {}
	virtual void slot6ValueChanged(double value) {}
	virtual void slot7ValueChanged(double value) {}
	virtual void slot8ValueChanged(double value) {}
	virtual void slot9ValueChanged(double value) {}
	virtual void slot10ValueChanged(double value) {}
	virtual void slot11ValueChanged(double value) {}
	virtual void slot12ValueChanged(double value) {}
	virtual void slot13ValueChanged(double value) {}
	virtual void slot14ValueChanged(double value) {}
	virtual void slot15ValueChanged(double value) {}
	virtual void slot16ValueChanged(double value) {}
};
