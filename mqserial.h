#ifndef MQSERIAL_H
#define MQSERIAL_H



/*POSIX CODE*/
#ifdef _TTY_POSIX_
#include "posix_qextserialport.h"
#define QextBaseType Posix_QextSerialPort

/*MS WINDOWS CODE*/
#else



#include <QMessageBox>
#include <qt_windows.h>
#include <QMainWindow>
class Readcom
{
public:
    Readcom();
    ~Readcom();
private:
     HKEY hKey;
    LPCWSTR subkey;

    char keyname[256]; //¼üÃûÊý×é
     char keyvalue[256];  //¼üÖµÊý×é
     DWORD keysize,type,valuesize;
    int indexnum;

public:
     QString getcomm(int index,QString keyorvalue); //¶ÁÈ¡¼üÃû
     //QString getvalue(int index);//¶ÁÈ¡¼üÖµ

    };



#include <QString>
#include <QList>
#pragma comment( lib, "Advapi32.lib" )              // added by csoapy
#ifdef _TTY_WIN_
    #include <windows.h>
    #include <setupapi.h>
#endif /*_TTY_WIN_*/


/*!
 * Structure containing port information.
 */
struct QextPortInfo {
    QString portName;		///< Port name.
    QString physName;		///< Physical name.
    QString friendName;		///< Friendly name.
    QString enumName;		///< Enumerator name.
};


/*!
 * Serial port enumerator. This class provides list of ports available in the system.
 *
 * Windows implementation is based on Zach Gorman's work from
 * <a href="http://www.codeproject.com">The Code Project</a> (http://www.codeproject.com/system/setupdi.asp).
 */
class QextSerialEnumerator
{
    private:
        #ifdef _TTY_WIN_
            /*!
             * Get value of specified property from the registry.
             * 	\param key handle to an open key.
             * 	\param property property name.
             * 	\return property value.
             */
            static QString getRegKeyValue(HKEY key, LPCTSTR property);

            /*!
             * Get specific property from registry.
             * 	\param devInfo pointer to the device information set that contains the interface
             * 		and its underlying device. Returned by SetupDiGetClassDevs() function.
             * 	\param devData pointer to an SP_DEVINFO_DATA structure that defines the device instance.
             * 		this is returned by SetupDiGetDeviceInterfaceDetail() function.
             * 	\param property registry property. One of defined SPDRP_* constants.
             * 	\return property string.
             */
            static QString getDeviceProperty(HDEVINFO devInfo, PSP_DEVINFO_DATA devData, DWORD property);

            /*!
             * Search for serial ports using setupapi.
             * 	\param infoList list with result.
             */
            static void setupAPIScan(QList<QextPortInfo> & infoList);
        #endif /*_TTY_WIN_*/

    public:
        /*!
         * Get list of ports.
         * 	\return list of ports currently available in the system.
         */
        static QList<QextPortInfo> getPorts();
};


#include <QIODevice>
#include <QFile>
#include <QThread>
#include <QMutex>

/*if all warning messages are turned off, flag portability warnings to be turned off as well*/
#ifdef _TTY_NOWARN_
#define _TTY_NOWARN_PORT_
#endif

/*macros for thread support*/
#define LOCK_MUTEX() mutex->lock()
#define UNLOCK_MUTEX() mutex->unlock()

/*macros for warning and debug messages*/
#ifdef _TTY_NOWARN_PORT_
#define TTY_PORTABILITY_WARNING(s)
#else
#define TTY_PORTABILITY_WARNING(s) qWarning(s)
#endif /*_TTY_NOWARN_PORT_*/
#ifdef _TTY_NOWARN_
#define TTY_WARNING(s)
#else
#define TTY_WARNING(s) qWarning(s)
#endif /*_TTY_NOWARN_*/


/*line status constants*/
#define LS_CTS  0x01
#define LS_DSR  0x02
#define LS_DCD  0x04
#define LS_RI   0x08
#define LS_RTS  0x10
#define LS_DTR  0x20
#define LS_ST   0x40
#define LS_SR   0x80

/*error constants*/
#define E_NO_ERROR                   0
#define E_INVALID_FD                 1
#define E_NO_MEMORY                  2
#define E_CAUGHT_NON_BLOCKED_SIGNAL  3
#define E_PORT_TIMEOUT               4
#define E_INVALID_DEVICE             5
#define E_BREAK_CONDITION            6
#define E_FRAMING_ERROR              7
#define E_IO_ERROR                   8
#define E_BUFFER_OVERRUN             9
#define E_RECEIVE_OVERFLOW          10
#define E_RECEIVE_PARITY_ERROR      11
#define E_TRANSMIT_OVERFLOW         12
#define E_READ_FAILED               13
#define E_WRITE_FAILED              14

/*!
 * Enums for port settings.
 */
enum NamingConvention
{
    WIN_NAMES,
    IRIX_NAMES,
    HPUX_NAMES,
    SUN_NAMES,
    DIGITAL_NAMES,
    FREEBSD_NAMES,
    LINUX_NAMES
};

enum BaudRateType
{
    BAUD50,                //POSIX ONLY
    BAUD75,                //POSIX ONLY
    BAUD110,
    BAUD134,               //POSIX ONLY
    BAUD150,               //POSIX ONLY
    BAUD200,               //POSIX ONLY
    BAUD300,
    BAUD600,
    BAUD1200,
    BAUD1800,              //POSIX ONLY
    BAUD2400,
    BAUD4800,
    BAUD9600,
    BAUD14400,             //WINDOWS ONLY
    BAUD19200,
    BAUD38400,
    BAUD56000,             //WINDOWS ONLY
    BAUD57600,
    BAUD76800,             //POSIX ONLY
    BAUD115200,
    BAUD128000,            //WINDOWS ONLY
    BAUD256000             //WINDOWS ONLY
};

enum DataBitsType
{
    DATA_5,
    DATA_6,
    DATA_7,
    DATA_8
};

enum ParityType
{
    PAR_NONE,
    PAR_ODD,
    PAR_EVEN,
    PAR_MARK,               //WINDOWS ONLY
    PAR_SPACE
};

enum StopBitsType
{
    STOP_1,
    STOP_1_5,               //WINDOWS ONLY
    STOP_2
};

enum FlowType
{
    FLOW_OFF,
    FLOW_HARDWARE,
    FLOW_XONXOFF
};

/**
 * structure to contain port settings
 */
struct PortSettings
{
    BaudRateType BaudRate;
    DataBitsType DataBits;
    ParityType Parity;
    StopBitsType StopBits;
    FlowType FlowControl;
    long Timeout_Millisec;
};

/*!
 * \author Stefan Sander
 * \author Michal Policht
 *
 * A common base class for Win_QextSerialBase, Posix_QextSerialBase and QextSerialPort.
 */
class QextSerialBase : public QIODevice
{
    Q_OBJECT

    public:
        enum QueryMode {
            Polling,
            EventDriven
        };

    protected:
        QMutex* mutex;
        QString port;
        PortSettings Settings;
        ulong lastErr;
        QextSerialBase::QueryMode _queryMode;

        virtual qint64 readData(char * data, qint64 maxSize)=0;
        virtual qint64 writeData(const char * data, qint64 maxSize)=0;

    public:
        QextSerialBase();
        QextSerialBase(const QString & name);
        virtual ~QextSerialBase();
        virtual void construct();
        virtual void setPortName(const QString & name);
        virtual QString portName() const;

        /**!
         * Get query mode.
         * 	\return query mode.
         */
        inline QextSerialBase::QueryMode queryMode() const { return _queryMode; };

        /*!
         * Set desired serial communication handling style. You may choose from polling
         * or event driven approach. This function does nothing when port is open; to
         * apply changes port must be reopened.
         *
         * In event driven approach read() and write() functions are acting
         * asynchronously. They return immediately and the operation is performed in
         * the background, so they doesn't freeze the calling thread.
         * To determine when operation is finished, QextSerialPort runs separate thread
         * and monitors serial port events. Whenever the event occurs, adequate signal
         * is emitted.
         *
         * When polling is set, read() and write() are acting synchronously. Signals are
         * not working in this mode and some functions may not be available. The advantage
         * of polling is that it generates less overhead due to lack of signals emissions
         * and it doesn't start separate thread to monitor events.
         *
         * Generally event driven approach is more capable and friendly, although some
         * applications may need as low overhead as possible and then polling comes.
         *
         * \param mode query mode.
         */
        virtual void setQueryMode(QueryMode mode);

//		virtual void setBlockingRead(bool block) = 0; ///< @todo implement.

        virtual void setBaudRate(BaudRateType)=0;
        virtual BaudRateType baudRate() const;
        virtual void setDataBits(DataBitsType)=0;
        virtual DataBitsType dataBits() const;
        virtual void setParity(ParityType)=0;
        virtual ParityType parity() const;
        virtual void setStopBits(StopBitsType)=0;
        virtual StopBitsType stopBits() const;
        virtual void setFlowControl(FlowType)=0;
        virtual FlowType flowControl() const;
        virtual void setTimeout(long)=0;

        virtual bool open(OpenMode mode)=0;
        virtual bool isSequential() const;
        virtual void close()=0;
        virtual void flush()=0;

        virtual qint64 size() const=0;
        virtual qint64 bytesAvailable()=0;
        virtual bool atEnd() const;

        virtual void ungetChar(char c)=0;
        virtual qint64 readLine(char * data, qint64 maxSize);

        virtual ulong lastError() const;
        virtual void translateError(ulong error)=0;

        virtual void setDtr(bool set=true)=0;
        virtual void setRts(bool set=true)=0;
        virtual ulong lineStatus()=0;

    signals:
        /**
         * This signal is emitted whenever port settings are updated.
         * 	\param valid \p true if settings are valid, \p false otherwise.
         *
         * 	@todo implement.
         */
//		void validSettings(bool valid);

        /*!
         * This signal is emitted whenever dsr line has changed its state. You may
         * use this signal to check if device is connected.
         * 	\param status \p true when DSR signal is on, \p false otherwise.
         *
         * 	\see lineStatus().
         */
        void dsrChanged(bool status);
};


#include <windows.h>
#include <QThread>


/*if all warning messages are turned off, flag portability warnings to be turned off as well*/
#ifdef _TTY_NOWARN_
#define _TTY_NOWARN_PORT_
#endif

class QReadWriteLock;
class Win_QextSerialThread;


/*!
\author Stefan Sander
\author Michal Policht

A cross-platform serial port class.
This class encapsulates the Windows portion of QextSerialPort.  The user will be notified of
errors and possible portability conflicts at run-time by default - this behavior can be turned
off by defining _TTY_NOWARN_ (to turn off all warnings) or _TTY_NOWARN_PORT_ (to turn off
portability warnings) in the project.  Note that defining _TTY_NOWARN_ also defines
_TTY_NOWARN_PORT_.

\note
On Windows NT/2000/XP this class uses Win32 serial port functions by default.  The user may
select POSIX behavior under NT, 2000, or XP ONLY by defining _TTY_POSIX_ in the project. I can
make no guarantees as to the quality of POSIX support under NT/2000 however.

\todo remove copy constructor and assign operator.
*/
class Win_QextSerialPort: public QextSerialBase
{
    Q_OBJECT
    friend class Win_QextSerialThread;

    private:
        /*!
         * This method is a part of constructor.
         */
        void init();

    protected:
        HANDLE Win_Handle;
        HANDLE threadStartEvent;
        HANDLE threadTerminateEvent;
        OVERLAPPED overlap;
        OVERLAPPED overlapWrite;
        COMMCONFIG Win_CommConfig;
        COMMTIMEOUTS Win_CommTimeouts;
        QReadWriteLock * bytesToWriteLock;	///< @todo maybe move to QextSerialBase.
        qint64 _bytesToWrite;		///< @todo maybe move to QextSerialBase (and implement in POSIX).
        Win_QextSerialThread * overlapThread; ///< @todo maybe move to QextSerialBase (and implement in POSIX).

        void monitorCommEvent();
        void terminateCommWait();
        virtual qint64 readData(char *data, qint64 maxSize);
        virtual qint64 writeData(const char *data, qint64 maxSize);

    public:
        Win_QextSerialPort();
        Win_QextSerialPort(Win_QextSerialPort const& s);
        Win_QextSerialPort(const QString & name, QextSerialBase::QueryMode mode = QextSerialBase::Polling);
        Win_QextSerialPort(const PortSettings& settings, QextSerialBase::QueryMode mode = QextSerialBase::Polling);
        Win_QextSerialPort(const QString & name, const PortSettings& settings, QextSerialBase::QueryMode mode = QextSerialBase::Polling);
        Win_QextSerialPort& operator=(const Win_QextSerialPort& s);
        virtual ~Win_QextSerialPort();
        virtual bool open(OpenMode mode);
        virtual void close();
        virtual void flush();
        virtual qint64 size() const;
        virtual void ungetChar(char c);
        virtual void setFlowControl(FlowType);
        virtual void setParity(ParityType);
        virtual void setDataBits(DataBitsType);
        virtual void setStopBits(StopBitsType);
        virtual void setBaudRate(BaudRateType);
        virtual void setDtr(bool set=true);
        virtual void setRts(bool set=true);
        virtual ulong lineStatus(void);
        virtual qint64 bytesAvailable();
        virtual void translateError(ulong);
        virtual void setTimeout(long);

        /*!
         * Return number of bytes waiting in the buffer. Currently this shows number
         * of bytes queued within write() and before the TX_EMPTY event occured. TX_EMPTY
         * event is created whenever last character in the system buffer was sent.
         *
         * \return number of bytes queued within write(), before the first TX_EMPTY
         * event occur.
         *
         * \warning this function may not give you expected results since TX_EMPTY may occur
         * while writing data to the buffer. Eventually some TX_EMPTY events may not be
         * catched.
         *
         * \note this function always returns 0 in polling mode.
         *
         * \see flush().
         */
        virtual qint64 bytesToWrite() const;

        virtual bool waitForReadyRead(int msecs);	///< @todo implement.
};

/*!
 * This thread monitors communication events.
 */
class Win_QextSerialThread: public QThread
{
    Win_QextSerialPort * qesp;
    bool terminate;

    public:
        /*!
         * Constructor.
         *
         * \param qesp valid serial port object.
         */
        Win_QextSerialThread(Win_QextSerialPort * qesp);

        /*!
         * Stop the thread.
         */
        void stop();

    protected:
        //overriden
        virtual void run();

};
#define QextBaseType Win_QextSerialPort
#endif

class QextSerialPort: public QextBaseType
{
    Q_OBJECT

    public:
        typedef QextSerialBase::QueryMode QueryMode;

        QextSerialPort();
        QextSerialPort(const QString & name, QueryMode mode = QextSerialPort::Polling);
        QextSerialPort(PortSettings const& s, QueryMode mode = QextSerialPort::Polling);
        QextSerialPort(const QString & name, PortSettings const& s, QueryMode mode = QextSerialPort::Polling);
        QextSerialPort(const QextSerialPort& s);
        QextSerialPort& operator=(const QextSerialPort&);
        virtual ~QextSerialPort();

};




























#include <QMainWindow>
#include "QLabel"
#include "QPlainTextEdit"
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include "QList"
#include <QLineEdit>
#include <QSlider>
#include <QSignalMapper>
#include <QStringListModel>
#include <QSettings>
namespace Ui {
class MQSerial;
}

class MQSerial : public QMainWindow
{
    Q_OBJECT

public:

     bool open_button_state;
    explicit MQSerial(QWidget *parent = 0);
    ~MQSerial();
public slots:
    void readMyCom();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();



    void on_comboBox_2_activated(int);



    void on_comboBox_3_activated(int index);

    void on_comboBox_5_activated(int index);



    void on_comboBox_4_activated(int index);

    void on_lineEdit_editingFinished();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_8_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_7_clicked();


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Win_QextSerialPort *myCom;
    Ui::MQSerial *ui;
    int prm_table[4];
    QextSerialEnumerator enumerator;
    long receivenum;
    Readcom comm;
    QString alltext;
    int sendnum;
    QLabel* btm_state;
    bool ishex(char a);
};

#endif // MQSERIAL_H
