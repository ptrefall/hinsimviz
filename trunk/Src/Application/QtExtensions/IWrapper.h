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
};
