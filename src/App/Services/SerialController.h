#pragma once

#include <QtSerialPort/QSerialPort>

#include <QTimer>
#include <QByteArray>
#include <QObject>
#include <QString>
#include <QVariantMap>

// Debugging catergories
#include "../Services/Debuging.h"

namespace App { namespace Services
{
    class SerialController: public QObject
    {
        Q_OBJECT

        public:
            explicit SerialController(QObject *parent = 0);
            ~SerialController();

            // Holds the current settings for the current connection
            QVariantMap     m_connectionValues;

            // What is the class responsable for?
            QString m_responsability;

            // What is the class responsable for?
            QString m_method;

            // When reading format hex to acsii
            bool m_hexToAcsii = false;

            // Find port name for device
            QString findPortName(quint16 productId, quint16 vendorID);

            // Bus communications
            bool open(QString com, int braud, int timeout, int parity=0);
            void close();
            bool write(const QByteArray &writeData);
            bool isOpen();

            // Validation helpers
            bool checkSumValidation(QStringList data, QString checkSum);

            // Signal package generators / helpers
            QVariantMap errorPackageGenerator(QString com, QString port, QString error);
            QVariantMap comConnectionPackageGenerator(QString com, bool status);

            // Check if bus in use
            bool busFree();
            bool checkDeviceAvaliable(bool reconnect = false);

        signals:
            void emit_critialSerialError(QVariantMap errorPackage);
            void emit_timeoutSerialError(QVariantMap errorPackage);
            void emit_comConnectionStatus(QVariantMap package);

        private slots:
            void handleRead();
            void handleBytesWritten(qint64 bytes);
            void handleTimeout();
            void handleError(QSerialPort::SerialPortError error);


        private:
            QSerialPort&    m_serialPort;
            QTimer&         m_timer;
            int             m_timeOut;
            qint64          m_bytesWritten;
            QByteArray      m_writeData;
            QStringList      m_readData;
            bool            m_busFree;

            void clearVars();

            // How the child should handle the read data
            virtual void proccessReadData(QStringList readData) = 0;

            // Validate the data to determin when the full package has been read
            virtual bool validate(QStringList data) = 0;

            // Due to number of check sum formats we'll abstract this out to the HALs
            virtual QString calculateCheckSum(QStringList string) = 0;

    };
}}


