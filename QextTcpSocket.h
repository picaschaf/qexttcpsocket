#ifndef QEXTTCPSOCKET_H
#define QEXTTCPSOCKET_H

#include <QTcpSocket>
#include <QString>
#include <QHostAddress>

class QTimer;


/**
 * \brief A QTcpSocket that is extended by a connect timeout.
 *
 * The default QTcpSocket class does not have any possibility to set a timeout when connecting to a host.
 * There are only two ways: 1. Use the blocking waitForConnected() method.
 *                          2. Use a timer and kill the connection attempt after a given time.
 *
 * In this implementation I encapsulate the second ways to achieve a timeout. Maybe in a later Qt version
 * there will be an option to set a connection timeout.
 */
class QextTcpSocket : public QTcpSocket
{
Q_OBJECT

public:
	explicit QextTcpSocket(QObject* parent = 0);
	virtual ~QextTcpSocket();

	// Methods from QTcpSocket overridden to catch a connect attempt
	void connectToHost(const QString&, quint16, OpenMode openMode = ReadWrite);
	void connectToHost(const QHostAddress&, quint16, OpenMode openMode = ReadWrite);


	void setTimeout(qint32 msec) { _timeout = msec; }
	qint32 timeout() const { return _timeout; }

private:
	QTimer* timeoutTimer;
	qint32  _timeout;

private slots:
	void timeout();
};

#endif // QEXTTCPSOCKET_H
