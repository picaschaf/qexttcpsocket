#include "QextTcpSocket.h"

#include <QTimer>
#include <QDebug>


/**
 *
 */
QextTcpSocket::QextTcpSocket(QObject* parent)
	: QTcpSocket(parent),
	  _timeout(-1)
{
	timeoutTimer = new QTimer(this);
	Q_CHECK_PTR(timeoutTimer);
	connect(timeoutTimer, SIGNAL(timeout()), this, SLOT(timeout()));
}

/**
 *
 */
QextTcpSocket::~QextTcpSocket()
{
}

/**
 * \brief Extends the QTcpSocket::connectToHost() method by starting an internal timer.
 *
 * If the timer was already started (multiple calls on connectToHost()), this will be detected
 * and ignored. Also, if the timeout time is less than 0 no timer will be started.
 *
 * \param hostName The destination's hostname
 * \param port     The destination's port
 * \param openMode Open the Socket for ReadOnly, ReadWrite or WriteOnly
 */
void QextTcpSocket::connectToHost(const QString& hostName, quint16 port, OpenMode openMode)
{
	if (timeoutTimer->isActive() == true) {
		qCritical() << "QextTcpSocket::connectToHost(): Connect is running...";
		return;
	}

	QTcpSocket::connectToHost(hostName, port, openMode);
	if (_timeout > 0)
		timeoutTimer->start(_timeout);
}

/**
 * \brief Extends the QTcpSocket::connectToHost() method by starting an internal timer.
 *
 * If the timer was already started (multiple calls on connectToHost()), this will be detected
 * and ignored. Also, if the timeout time is less than 0 no timer will be started.
 *
 * \param hostAddress The destination's hostaddress
 * \param port        The destination's port
 * \param openMode    Open the Socket for ReadOnly, ReadWrite or WriteOnly
 */
void QextTcpSocket::connectToHost(const QHostAddress& hostAddress, quint16 port, OpenMode openMode)
{
	if (timeoutTimer->isActive() == true) {
		qCritical() << "QextTcpSocket::connectToHost(): Connect is running...";
		return;
	}

	QTcpSocket::connectToHost(hostAddress, port, openMode);
	if (_timeout > 0)
		timeoutTimer->start(_timeout);
}

/**
 * \brief This internal slot is used to detect the timeout and emit an error if the socket is not connected.
 */
void QextTcpSocket::timeout()
{
	timeoutTimer->stop();
	close();

	if (state() != ConnectedState)
		emit error(SocketTimeoutError);
}
