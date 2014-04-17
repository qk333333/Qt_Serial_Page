#include "mqserial.h"

#include "ui_mainwindow.h"
#include "QTextCodec"
#include <QtCore/QSettings>
#include <stdlib.h>
#include <stdio.h>
#include <QDialog>
#include <QFileDialog>
#include <fstream>
#include <stdlib.h>
#include "winbase.h"
#include "QPlainTextEdit"
#include "ui_mqserial.h"
bool ishex(char);
MQSerial::MQSerial(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MQSerial)
{

    ui->setupUi(this);

    open_button_state=false;
    receivenum =0;
    sendnum=0;

    prm_table[0]=19;
    prm_table[1]=3;
    prm_table[2]=0;
    prm_table[3]=0;

    ui->pushButton_3->setEnabled(false);
    ui->pushButton_2->setEnabled(false);


    myCom = new Win_QextSerialPort("\\\\.\\com11", QextSerialBase::EventDriven);
    connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));

// to read default settings
//    QString path="HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM";
//    QSettings *settings=new QSettings(path,QSettings::NativeFormat);
//    QStringList key=settings->allKeys();



    for(int i=0;comm.getcomm(i,"value")!="nokey"&&comm.getcomm(i,"value")!="Cannot open regedit!";i++)
    {
      ui->comboBox->addItem(comm.getcomm(i,"value"));
    }


}

MQSerial::~MQSerial()
{
    delete ui;
}
void MQSerial::readMyCom()

{
    QByteArray temp = myCom->readAll();
    receivenum+=temp.size();

    QString delta2;
    delta2=QString("%1").arg(receivenum);
    ui->lineEdit_3->setText(delta2);


    if(!ui->checkBox->isChecked())
    {
        alltext.append(temp);
        int sizesize=temp.size();
        QString ss=temp.toHex();
        for(long i=0;i<sizesize;i++)
            ss.insert(3*i," ");


        if(ui->checkBox_2->isChecked())
            ui->plainTextEdit->insertPlainText(ss);
        else
            ui->plainTextEdit->insertPlainText(temp);

       ui->plainTextEdit->moveCursor(QTextCursor::End);
    }
}



void MQSerial::on_pushButton_4_clicked()
{
    if(open_button_state)
    {
        if(myCom->isOpen())
        {
            myCom->close();/*may occur "WaitCommEvent error 22".
 I don't know how to solve.*/
            ui->pushButton_4->setText("Open");
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_8->setEnabled(false);
            open_button_state=!open_button_state;
        }
    }
    else
    {
        ui->pushButton_8->setEnabled(true);
        QString ss="\\\\.\\com";
        QString zhongjian=ui->comboBox->currentText();
        ss.append(zhongjian[3]);
        if(zhongjian.length()>4)
        ss.append(zhongjian[4]);
        if(zhongjian.length()>5)
        ss.append(zhongjian[5]);
        myCom->setPortName(ss);
        if(myCom ->open(QIODevice::ReadWrite))
        {
            ui->pushButton_4->setText("Close");
            ui->pushButton_3->setEnabled(true);
            ui->pushButton_2->setEnabled(true);
            myCom->setBaudRate((BaudRateType)prm_table[0]);
            myCom->setDataBits((DataBitsType)prm_table[1]);
            myCom->setParity((ParityType)prm_table[2]);
            myCom->setStopBits((StopBitsType)prm_table[3]);
             open_button_state=!open_button_state;
        }


    }
}

void MQSerial::on_pushButton_3_clicked()
{
    if(!ui->checkBox_3->isChecked())
    {
        if(ui->checkBox_10->isChecked())
        {
            myCom->write(ui->lineEdit->text().toLatin1().append("\n"));
            sendnum+=ui->lineEdit->text().toLatin1().size()+1;
        }
        else
        {
            myCom->write(ui->lineEdit->text().toLatin1());
            sendnum+=ui->lineEdit->text().toLatin1().size();
        }
    }
    else
    {
        QByteArray fasonghex=ui->lineEdit->text().toLatin1();
        QString fasongzhi;
        int indexjishu=0;
        QString fasongxinxi;
        while(1)
        {


            if(indexjishu+1>=fasonghex.size())
                break;
            {

                char a[3];
                if(ishex(fasonghex[indexjishu]))
                a[0]=fasonghex[indexjishu];
                else
                {
                    QString aa=tr("ŐâĘÇHEXˇ˘ËÍĹśÇ×ŁŹÄúĘäČë´íż� ");


                break;
                }
                if(ishex(fasonghex[indexjishu+1]))
                a[1]=fasonghex[indexjishu+1];
                else
                {
                    QString aa=tr("ŐâĘÇHEXˇ˘ËÍĹśÇ×ŁŹÄúĘäČë´íż� ");
                    //cuowutishi->ui->label->setText(aa);
                    //cuowutishi->show();
                break;
                }
                a[2]='\0';
                QByteArray text = QByteArray::fromHex(a);
                fasongxinxi.append(text.data());


                if(fasonghex[indexjishu+2]==' ')
                indexjishu+=3;
                else
                indexjishu+=2;


            }
        }
        if(ui->checkBox_10->isChecked())
        {
            myCom->write(fasongxinxi.insert(0,"\n").toLatin1());
            sendnum+=indexjishu/3+1+1;
        }
        else
        {
            myCom->write(fasongxinxi.toLatin1());
            sendnum+=indexjishu/3+1;
        }

    }
    QString delta3;
    delta3=QString("%1").arg(sendnum);
    ui->lineEdit_3->setText(delta3);

}

void MQSerial::on_comboBox_2_activated(int a)
{
    myCom->setBaudRate((BaudRateType)a);
    prm_table[0]=a;
}

void MQSerial::on_comboBox_3_activated(int index)
{
    myCom->setDataBits((DataBitsType)index);
    prm_table[1]=index;
}

void MQSerial::on_comboBox_5_activated(int index)
{
    myCom->setParity((ParityType)index);
    prm_table[2]=index;
}

void MQSerial::on_comboBox_4_activated(int index)
{
    myCom->setStopBits((StopBitsType)index );
    prm_table[3]=index;
}

void MQSerial::on_lineEdit_editingFinished()
{
   // myCom->write(ui->lineEdit->text().toAscii());
}

void MQSerial::on_lineEdit_textChanged(const QString &arg1)
{
    QString aa=arg1;
    if(ui->lineEdit->text().size()==0)
        ui->pushButton_3->setEnabled(false);
    else
        ui->pushButton_3->setEnabled(true);

}
bool MQSerial::ishex(char a)
{
    if((a>='a'&&a<='f')||(a>='A'&&a<='F')||((a>='0')&&(a<='9')))
        return true;
    else return false;
}

void MQSerial::on_pushButton_8_clicked()
{
    ui->comboBox->clear();
    for(int i=0;comm.getcomm(i,"value")!="nokey";i++)
    {
      ui->comboBox->addItem(comm.getcomm(i,"value"));
    }

}

void MQSerial::on_pushButton_6_clicked()
{
    ui->plainTextEdit->moveCursor(QTextCursor::Start);
}

void MQSerial::on_pushButton_5_clicked()
{
    ui->plainTextEdit->clear();
    alltext.clear();
    receivenum=0;
    sendnum=0;
}

void MQSerial::on_checkBox_2_stateChanged(int arg1)
{
        //ŃĄÖĐÎŞ2
    ui->plainTextEdit->clear();
    int textsize=alltext.toLatin1().size();
    QString zhongjie=alltext.toLatin1().toHex();
    for(long i=0;i<textsize;i++)
        zhongjie.insert(3*i," ");
    if(arg1==2)
    ui->plainTextEdit->appendPlainText(zhongjie);
    else
        ui->plainTextEdit->appendPlainText(alltext);
    this->isAnimated();
}


void MQSerial::on_checkBox_7_clicked()
{

}


void MQSerial::on_pushButton_2_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr(""), ".", tr("all Files(*.*)"));
          if(path.length() == 0) {
                  QMessageBox::information(NULL, tr(""), tr(""));
          } else {
                  QMessageBox::information(NULL, tr(""), tr("~") + path);
          }
          QFile aa(path);
          if( aa.open(QIODevice::ReadOnly))
          {
         //   QTextStream textStream(&aa);
           // while(!textStream.atEnd())
           // {
             ui->plainTextEdit->appendPlainText(aa.readAll().toHex());
             if(!ui->checkBox_3->isChecked())
             {
                 QByteArray fasonghex=aa.readAll();

                 if(ui->checkBox_10->isChecked())
                 {
                     myCom->write(aa.readAll().append("\n"));

                 }
                 else
                 {
                     myCom->write(aa.readAll());

                 }
             }
             else
             {
                 QByteArray fasonghex=aa.read(1000000000);

                 QString fasongzhi;
                 int indexjishu=0;
                 QString fasongxinxi;
                 while(1)
                 {


                     if(indexjishu+1>=fasonghex.size())
                         break;
                     {

                         char a[3];
                         if(ishex(fasonghex[indexjishu]))
                         a[0]=fasonghex[indexjishu];
                         else
                         {
                             QString aa=tr(" ");
                             //cuowutishi->ui->label->setText(aa);
                             //cuowutishi->show();

                         break;
                         }
                         if(ishex(fasonghex[indexjishu+1]))
                         a[1]=fasonghex[indexjishu+1];
                         else
                         {
                             QString aa=tr(" ");
                             //cuowutishi->ui->label->setText(aa);
                             //cuowutishi->show();
                         break;
                         }
                         a[2]='\0';
                         QByteArray text = QByteArray::fromHex(a);
                         fasongxinxi.append(text.data());


                         if(fasonghex[indexjishu+2]==' ')
                         indexjishu+=3;
                         else
                         indexjishu+=2;


                     }
                 }
                 if(ui->checkBox_10->isChecked())
                 {
                     myCom->write(fasongxinxi.insert(0,"\n").toLatin1());

                 }
                 else
                 {
                     myCom->write(fasongxinxi.toLatin1());

                 }

             }

           // }

          }
        QString delta;
        sendnum+=ui->plainTextEdit->textCursor().position()/2;
        delta=QString("%1").arg(sendnum);
        ui->lineEdit_2->setText(delta);

          aa.close();

}

void MQSerial::on_pushButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr(""), ".", tr("all Files(*.*)"));
          if(path.length() == 0) {
                  QMessageBox::information(NULL, tr(""), tr(""));
          } else {
                  QMessageBox::information(NULL, tr(""), tr("~") + path);
          }
          QFile bb(path);
          if(bb.open(QIODevice::WriteOnly))
          {
            bb.write(ui->plainTextEdit->toPlainText().toLatin1());
          }
          bb.close();

}





















/*!
\class QextSerialPort
\author Stefan Sander
\author Michal Policht

A cross-platform serial port class.
This class encapsulates a serial port on both POSIX and Windows systems.  The user will be
notified of errors and possible portability conflicts at run-time by default - this behavior can
be turned off by defining _TTY_NOWARN_ (to turn off all warnings) or _TTY_NOWARN_PORT_ (to turn
off portability warnings) in the project.

You may choose from polling or event driven API. For details check setQueryMode() documentation.

\note
On Windows NT/2000/XP this class uses Win32 serial port functions by default.  The user may
select POSIX behavior under NT, 2000, or XP ONLY by defining _TTY_POSIX_ in the project. I can
make no guarantees as to the quality of POSIX support under NT/2000 however.

*/

#include <stdio.h>
//#include "qextserialport.h"



#include <objbase.h>
#include <initguid.h>


#include <QReadWriteLock>

/*!
Default constructor.  Note that the naming convention used by a QextSerialPort constructed with
this constructor will be determined by #defined constants, or lack thereof - the default behavior
is the same as _TTY_LINUX_.  Possible naming conventions and their associated constants are:

\verbatim

Constant         Used By         Naming Convention
----------       -------------   ------------------------
_TTY_WIN_        Windows         COM1, COM2
_TTY_IRIX_       SGI/IRIX        /dev/ttyf1, /dev/ttyf2
_TTY_HPUX_       HP-UX           /dev/tty1p0, /dev/tty2p0
_TTY_SUN_        SunOS/Solaris   /dev/ttya, /dev/ttyb
_TTY_DIGITAL_    Digital UNIX    /dev/tty01, /dev/tty02
_TTY_FREEBSD_    FreeBSD         /dev/ttyd0, /dev/ttyd1
_TTY_LINUX_      Linux           /dev/ttyS0, /dev/ttyS1
<none>           Linux           /dev/ttyS0, /dev/ttyS1
\endverbatim

The object will be associated with the first port in the system, e.g. COM1 on Windows systems.
See the other constructors if you need to use a port other than the first.
*/
QextSerialPort::QextSerialPort()
 : QextBaseType()
{}

/*!
Constructs a serial port attached to the port specified by name.
name is the name of the device, which is windowsystem-specific,
e.g."COM1" or "/dev/ttyS0".

\see setQueryMode().
*/
QextSerialPort::QextSerialPort(const QString & name, QueryMode mode)
 : QextBaseType(name, mode)
{
}

/*!
Constructs a port with default name and settings specified by the settings parameter.

\see setQueryMode().
*/
QextSerialPort::QextSerialPort(PortSettings const& settings, QueryMode mode)
 : QextBaseType(settings, mode)
{}

/*!
Constructs a port with the name and settings specified.

\see setQueryMode().
*/
QextSerialPort::QextSerialPort(const QString & name, PortSettings const& settings, QueryMode mode)
 : QextBaseType(name, settings, mode)
{}

/*!
Copy constructor.

\deprecated
*/
QextSerialPort::QextSerialPort(const QextSerialPort& s)
 : QextBaseType(s)
{}

/*!
\fn QextSerialPort& QextSerialPort::operator=(const QextSerialPort& s)
Overrides the = operator.

\deprecated
*/
QextSerialPort& QextSerialPort::operator=(const QextSerialPort& s)
{
    return (QextSerialPort&)QextBaseType::operator=(s);
}

/*!
\fn QextSerialPort::~QextSerialPort()
Standard destructor.
*/
QextSerialPort::~QextSerialPort()
{}





























Readcom::Readcom()
{

}


Readcom::~Readcom()
{

}

QString Readcom::getcomm(int index,QString keyorvalue)
{
    QString commresult="";
    QString strkey="HARDWARE\\DEVICEMAP\\SERIALCOMM";//×Ó¼üÂ·¾¶
    subkey=(LPCWSTR)strkey.utf16();//ÀàÐÍ×ª»»

    if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,subkey,0,KEY_READ,&hKey)!=0)
    {
        QString error="Cannot open regedit!";//ÎÞ·¨´ò¿ª×¢²á±íÊ±·µ»Øerror
        return error;
    }//if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,subkey,0,KEY_READ,&hKey)!=0)

    QString keymessage="";//¼üÃû
    QString message;
    QString valuemessage="";//¼üÖµ
    indexnum=index;//Òª¶ÁÈ¡¼üÖµµÄË÷ÒýºÅ

    keysize=sizeof(keyname);
    valuesize=sizeof(keyvalue);

    if(::RegEnumValue(hKey,indexnum,(LPWSTR)keyname,&keysize,0,&type,(BYTE*)keyvalue,&valuesize)==0)//ÁÐ¾Ù¼üÃûºÍÖµ
    {
        for(unsigned i=0;i<keysize;i++)
        {
            message=keyname;
            keymessage.append(message);
        }// for(int i=0;i<=keysize;i++)    ¶ÁÈ¡¼üÃû
        for(unsigned int j=0;j<valuesize;j++)
        {
            if(keyvalue[j]!=0x00)
            { valuemessage.append(keyvalue[j]);}
            }//for(int j=0;j<valuesize;j++) ¶ÁÈ¡¼üÖµ
        if(keyorvalue=="key")
        {
          commresult=keymessage;
        }
        if(keyorvalue=="value")
        {
         commresult=valuemessage;
        }

    }
    else
    {
     commresult="nokey";
    } //if(::RegEnumValue(hKey,indexnum,keyname,&keysize,0,&type,(BYTE*)keyvalue,&valuesize)==0) ÁÐ¾Ù¼üÃûºÍÖµ

    return commresult;
    ::RegCloseKey(hKey);//¹Ø±Õ×¢²á±í
}









































/**
 * @file qextserialenumerator.cpp
 * @author Michał Policht
 * @see QextSerialEnumerator
 */



#ifdef _TTY_WIN_
    //this is serial port GUID
    #ifndef GUID_CLASS_COMPORT
        DEFINE_GUID(GUID_CLASS_COMPORT, 0x86e0d1e0L, 0x8089, 0x11d0, 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73);
    #endif

    /* Gordon Schumacher's macros for TCHAR -> QString conversions and vice versa */
    #ifdef UNICODE
        #define QStringToTCHAR(x)     (wchar_t*) x.utf16()
        #define PQStringToTCHAR(x)    (wchar_t*) x->utf16()
        #define TCHARToQString(x)     QString::fromUtf16((ushort*)(x))
        #define TCHARToQStringN(x,y)  QString::fromUtf16((ushort*)(x),(y))
    #else
        #define QStringToTCHAR(x)     x.local8Bit().constData()
        #define PQStringToTCHAR(x)    x->local8Bit().constData()
        #define TCHARToQString(x)     QString::fromLocal8Bit((x))
        #define TCHARToQStringN(x,y)  QString::fromLocal8Bit((x),(y))
    #endif /*UNICODE*/


    //static
    QString QextSerialEnumerator::getRegKeyValue(HKEY key, LPCTSTR property)
    {
        DWORD size = 0;
        RegQueryValueEx(key, property, NULL, NULL, NULL, & size);
        BYTE * buff = new BYTE[size];
        if (RegQueryValueEx(key, property, NULL, NULL, buff, & size) == ERROR_SUCCESS) {
            return TCHARToQStringN(buff, size);
            delete [] buff;
        } else {
            qWarning("QextSerialEnumerator::getRegKeyValue: can not obtain value from registry");
            delete [] buff;
            return QString();
        }
    }

    //static
    QString QextSerialEnumerator::getDeviceProperty(HDEVINFO devInfo, PSP_DEVINFO_DATA devData, DWORD property)
    {
        DWORD buffSize = 0;
        SetupDiGetDeviceRegistryProperty(devInfo, devData, property, NULL, NULL, 0, & buffSize);
        BYTE * buff = new BYTE[buffSize];
        if (!SetupDiGetDeviceRegistryProperty(devInfo, devData, property, NULL, buff, buffSize, NULL))
            qCritical("Can not obtain property: %ld from registry", property);
        QString result = TCHARToQString(buff);
        delete [] buff;
        return result;
    }

    //static
    void QextSerialEnumerator::setupAPIScan(QList<QextPortInfo> & infoList)
    {
        HDEVINFO devInfo = INVALID_HANDLE_VALUE;
        GUID * guidDev = (GUID *) & GUID_CLASS_COMPORT;

        devInfo = SetupDiGetClassDevs(guidDev, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
        if(devInfo == INVALID_HANDLE_VALUE) {
            qCritical("SetupDiGetClassDevs failed. Error code: %ld", GetLastError());
            return;
        }

        //enumerate the devices
        bool ok = true;
        SP_DEVICE_INTERFACE_DATA ifcData;
        ifcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
        SP_DEVICE_INTERFACE_DETAIL_DATA * detData = NULL;
        DWORD detDataSize = 0;
        DWORD oldDetDataSize = 0;

        for (DWORD i = 0; ok; i++) {
            ok = SetupDiEnumDeviceInterfaces(devInfo, NULL, guidDev, i, &ifcData);
            if (ok) {
                SP_DEVINFO_DATA devData = {sizeof(SP_DEVINFO_DATA)};
                //check for required detData size
                SetupDiGetDeviceInterfaceDetail(devInfo, & ifcData, NULL, 0, & detDataSize, & devData);
                //if larger than old detData size then reallocate the buffer
                if (detDataSize > oldDetDataSize) {
                    delete [] detData;
                    detData = (SP_DEVICE_INTERFACE_DETAIL_DATA *) new char[detDataSize];
                    detData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
                    oldDetDataSize = detDataSize;
                }
                //check the details
                if (SetupDiGetDeviceInterfaceDetail(devInfo, & ifcData, detData, detDataSize,
                                                    NULL, & devData)) {
                    // Got a device. Get the details.
                    QextPortInfo info;
                    info.friendName = getDeviceProperty(devInfo, & devData, SPDRP_FRIENDLYNAME);
                    info.physName = getDeviceProperty(devInfo, & devData, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME);
                    info.enumName = getDeviceProperty(devInfo, & devData, SPDRP_ENUMERATOR_NAME);
                    //anyway, to get the port name we must still open registry directly :( ???
                    //Eh...
                    HKEY devKey = SetupDiOpenDevRegKey(devInfo, & devData, DICS_FLAG_GLOBAL, 0,
                                                        DIREG_DEV, KEY_READ);
                    info.portName = getRegKeyValue(devKey, TEXT("PortName"));
                    RegCloseKey(devKey);
                    infoList.append(info);
                } else {
                    qCritical("SetupDiGetDeviceInterfaceDetail failed. Error code: %ld", GetLastError());
                    delete [] detData;
                    return;
                }
            } else {
                if (GetLastError() != ERROR_NO_MORE_ITEMS) {
                    delete [] detData;
                    qCritical("SetupDiEnumDeviceInterfaces failed. Error code: %ld", GetLastError());
                    return;
                }
            }
        }
        delete [] detData;
    }

#endif /*_TTY_WIN_*/


//static
QList<QextPortInfo> QextSerialEnumerator::getPorts()
{
    QList<QextPortInfo> ports;

    #ifdef _TTY_WIN_
        OSVERSIONINFO vi;
        vi.dwOSVersionInfoSize = sizeof(vi);
        if (!::GetVersionEx(&vi)) {
            qCritical("Could not get OS version.");
            return ports;
        }
        // Handle windows 9x and NT4 specially
        if (vi.dwMajorVersion < 5) {
            qCritical("Enumeration for this version of Windows is not implemented yet");
/*			if (vi.dwPlatformId == VER_PLATFORM_WIN32_NT)
                EnumPortsWNt4(ports);
            else
                EnumPortsW9x(ports);*/
        } else	//w2k or later
            setupAPIScan(ports);
    #endif /*_TTY_WIN_*/
    #ifdef _TTY_POSIX_
        qCritical("Enumeration for POSIX systems is not implemented yet.");
    #endif /*_TTY_POSIX_*/

    return ports;
}




































/*!
\fn QextSerialBase::QextSerialBase()
Default constructor.
*/
QextSerialBase::QextSerialBase()
 : QIODevice()
{

#ifdef _TTY_WIN_
    setPortName("COM1");

#elif defined(_TTY_IRIX_)
    setPortName("/dev/ttyf1");

#elif defined(_TTY_HPUX_)
    setPortName("/dev/tty1p0");

#elif defined(_TTY_SUN_)
    setPortName("/dev/ttya");

#elif defined(_TTY_DIGITAL_)
    setPortName("/dev/tty01");

#elif defined(_TTY_FREEBSD_)
    setPortName("/dev/ttyd1");

#else
    setPortName("/dev/ttyS0");
#endif

    construct();
}

/*!
\fn QextSerialBase::QextSerialBase(const QString & name)
Construct a port and assign it to the device specified by the name parameter.
*/
QextSerialBase::QextSerialBase(const QString & name)
 : QIODevice()
{
    setPortName(name);
    construct();
}

/*!
\fn QextSerialBase::~QextSerialBase()
Standard destructor.
*/
QextSerialBase::~QextSerialBase()
{
    delete mutex;
}

/*!
\fn void QextSerialBase::construct()
Common constructor function for setting up default port settings.
(115200 Baud, 8N1, Hardware flow control where supported, otherwise no flow control, and 0 ms timeout).
*/
void QextSerialBase::construct()
{
    Settings.BaudRate=BAUD115200;
    Settings.DataBits=DATA_8;
    Settings.Parity=PAR_NONE;
    Settings.StopBits=STOP_1;
    Settings.FlowControl=FLOW_HARDWARE;
    Settings.Timeout_Millisec=500;
    mutex = new QMutex( QMutex::Recursive );
    setOpenMode(QIODevice::NotOpen);
}

void QextSerialBase::setQueryMode(QueryMode mechanism)
{
    _queryMode = mechanism;
}

/*!
\fn void QextSerialBase::setPortName(const QString & name)
Sets the name of the device associated with the object, e.g. "COM1", or "/dev/ttyS0".
*/
void QextSerialBase::setPortName(const QString & name)
{
    port = name;
}

/*!
\fn QString QextSerialBase::portName() const
Returns the name set by setPortName().
*/
QString QextSerialBase::portName() const
{
    return port;
}

/*!
\fn BaudRateType QextSerialBase::baudRate(void) const
Returns the baud rate of the serial port.  For a list of possible return values see
the definition of the enum BaudRateType.
*/
BaudRateType QextSerialBase::baudRate(void) const
{
    return Settings.BaudRate;
}

/*!
\fn DataBitsType QextSerialBase::dataBits() const
Returns the number of data bits used by the port.  For a list of possible values returned by
this function, see the definition of the enum DataBitsType.
*/
DataBitsType QextSerialBase::dataBits() const
{
    return Settings.DataBits;
}

/*!
\fn ParityType QextSerialBase::parity() const
Returns the type of parity used by the port.  For a list of possible values returned by
this function, see the definition of the enum ParityType.
*/
ParityType QextSerialBase::parity() const
{
    return Settings.Parity;
}

/*!
\fn StopBitsType QextSerialBase::stopBits() const
Returns the number of stop bits used by the port.  For a list of possible return values, see
the definition of the enum StopBitsType.
*/
StopBitsType QextSerialBase::stopBits() const
{
    return Settings.StopBits;
}

/*!
\fn FlowType QextSerialBase::flowControl() const
Returns the type of flow control used by the port.  For a list of possible values returned
by this function, see the definition of the enum FlowType.
*/
FlowType QextSerialBase::flowControl() const
{
    return Settings.FlowControl;
}

/*!
\fn bool QextSerialBase::isSequential() const
Returns true if device is sequential, otherwise returns false. Serial port is sequential device
so this function always returns true. Check QIODevice::isSequential() documentation for more
information.
*/
bool QextSerialBase::isSequential() const
{
    return true;
}

/*!
\fn bool QextSerialBase::atEnd() const
This function will return true if the input buffer is empty (or on error), and false otherwise.
Call QextSerialBase::lastError() for error information.
*/
bool QextSerialBase::atEnd() const
{
    if (size()) {
        return true;
    }
    return false;
}

/*!
\fn qint64 QextSerialBase::readLine(char * data, qint64 maxSize)
This function will read a line of buffered input from the port, stopping when either maxSize bytes
have been read, the port has no more data available, or a newline is encountered.
The value returned is the length of the string that was read.
*/
qint64 QextSerialBase::readLine(char * data, qint64 maxSize)
{
    qint64 numBytes = bytesAvailable();
    char* pData = data;

    if (maxSize < 2)	//maxSize must be larger than 1
        return -1;

    /*read a byte at a time for MIN(bytesAvail, maxSize - 1) iterations, or until a newline*/
    while (pData<(data+numBytes) && --maxSize) {
        readData(pData, 1);
        if (*pData++ == '\n') {
            break;
        }
    }
    *pData='\0';

    /*return size of data read*/
    return (pData-data);
}

/*!
\fn ulong QextSerialBase::lastError() const
Returns the code for the last error encountered by the port, or E_NO_ERROR if the last port
operation was successful.  Possible error codes are:

\verbatim
Error                           Explanation
---------------------------     -------------------------------------------------------------
E_NO_ERROR                      No Error has occured
E_INVALID_FD                    Invalid file descriptor (port was not opened correctly)
E_NO_MEMORY                     Unable to allocate memory tables (POSIX)
E_CAUGHT_NON_BLOCKED_SIGNAL     Caught a non-blocked signal (POSIX)
E_PORT_TIMEOUT                  Operation timed out (POSIX)
E_INVALID_DEVICE                The file opened by the port is not a character device (POSIX)
E_BREAK_CONDITION               The port detected a break condition
E_FRAMING_ERROR                 The port detected a framing error
                                (usually caused by incorrect baud rate settings)
E_IO_ERROR                      There was an I/O error while communicating with the port
E_BUFFER_OVERRUN                Character buffer overrun
E_RECEIVE_OVERFLOW              Receive buffer overflow
E_RECEIVE_PARITY_ERROR          The port detected a parity error in the received data
E_TRANSMIT_OVERFLOW             Transmit buffer overflow
E_READ_FAILED                   General read operation failure
E_WRITE_FAILED                  General write operation failure
\endverbatim
*/
ulong QextSerialBase::lastError() const
{
    return lastErr;
}


















































//#include <stdio.h>
//#include <Process.h>
//#include <QCoreApplication>
//#include <QEvent>



/*!
\fn Win_QextSerialPort::Win_QextSerialPort()
Default constructor.  Note that the name of the device used by a Win_QextSerialPort constructed
with this constructor will be determined by #defined constants, or lack thereof - the default
behavior is the same as _TTY_LINUX_.  Possible naming conventions and their associated constants
are:

\verbatim

Constant         Used By         Naming Convention
----------       -------------   ------------------------
_TTY_WIN_        Windows         COM1, COM2
_TTY_IRIX_       SGI/IRIX        /dev/ttyf1, /dev/ttyf2
_TTY_HPUX_       HP-UX           /dev/tty1p0, /dev/tty2p0
_TTY_SUN_        SunOS/Solaris   /dev/ttya, /dev/ttyb
_TTY_DIGITAL_    Digital UNIX    /dev/tty01, /dev/tty02
_TTY_FREEBSD_    FreeBSD         /dev/ttyd0, /dev/ttyd1
_TTY_LINUX_      Linux           /dev/ttyS0, /dev/ttyS1
<none>           Linux           /dev/ttyS0, /dev/ttyS1
\endverbatim

This constructor associates the object with the first port on the system, e.g. COM1 for Windows
platforms.  See the other constructor if you need a port other than the first.
*/
Win_QextSerialPort::Win_QextSerialPort():
    QextSerialBase()
{
    Win_Handle=INVALID_HANDLE_VALUE;
    init();
}

/*!Win_QextSerialPort::Win_QextSerialPort(const Win_QextSerialPort&)
Copy constructor.
*/
Win_QextSerialPort::Win_QextSerialPort(const Win_QextSerialPort& s):
    QextSerialBase(s.port)
{
    Win_Handle=INVALID_HANDLE_VALUE;
    _queryMode = s._queryMode;
    _bytesToWrite = s._bytesToWrite;
    bytesToWriteLock = new QReadWriteLock;
    overlapThread = new Win_QextSerialThread(this);
    memcpy(& overlap, & s.overlap, sizeof(OVERLAPPED));
    memcpy(& overlapWrite, & s.overlapWrite, sizeof(OVERLAPPED));
    setOpenMode(s.openMode());
    lastErr=s.lastErr;
    port = s.port;
    Settings.FlowControl=s.Settings.FlowControl;
    Settings.Parity=s.Settings.Parity;
    Settings.DataBits=s.Settings.DataBits;
    Settings.StopBits=s.Settings.StopBits;
    Settings.BaudRate=s.Settings.BaudRate;
    Win_Handle=s.Win_Handle;
    memcpy(&Win_CommConfig, &s.Win_CommConfig, sizeof(COMMCONFIG));
    memcpy(&Win_CommTimeouts, &s.Win_CommTimeouts, sizeof(COMMTIMEOUTS));
    if (s.overlapThread->isRunning())
        overlapThread->start();
}

/*!
\fn Win_QextSerialPort::Win_QextSerialPort(const QString & name)
Constructs a serial port attached to the port specified by devName.
devName is the name of the device, which is windowsystem-specific,
e.g."COM2" or "/dev/ttyS0".
*/
Win_QextSerialPort::Win_QextSerialPort(const QString & name, QextSerialBase::QueryMode mode):
    QextSerialBase(name)
{
    Win_Handle=INVALID_HANDLE_VALUE;
    setQueryMode(mode);
    init();
}

/*!
\fn Win_QextSerialPort::Win_QextSerialPort(const PortSettings& settings)
Constructs a port with default name and specified settings.
*/
Win_QextSerialPort::Win_QextSerialPort(const PortSettings& settings, QextSerialBase::QueryMode mode) {
    Win_Handle=INVALID_HANDLE_VALUE;
    setBaudRate(settings.BaudRate);
    setDataBits(settings.DataBits);
    setStopBits(settings.StopBits);
    setParity(settings.Parity);
    setFlowControl(settings.FlowControl);
    setTimeout(settings.Timeout_Millisec);
    setQueryMode(mode);
    init();
}

/*!
\fn Win_QextSerialPort::Win_QextSerialPort(const QString & name, const PortSettings& settings)
Constructs a port with specified name and settings.
*/
Win_QextSerialPort::Win_QextSerialPort(const QString & name, const PortSettings& settings, QextSerialBase::QueryMode mode) {
    Win_Handle=INVALID_HANDLE_VALUE;
    setPortName(name);
    setBaudRate(settings.BaudRate);
    setDataBits(settings.DataBits);
    setStopBits(settings.StopBits);
    setParity(settings.Parity);
    setFlowControl(settings.FlowControl);
    setTimeout(settings.Timeout_Millisec);
    setQueryMode(mode);
    init();
}
void Win_QextSerialPort::init()
{
    _bytesToWrite = 0;
    overlap.Internal = 0;
    overlap.InternalHigh = 0;
    overlap.Offset = 0;
    overlap.OffsetHigh = 0;
    overlap.hEvent = CreateEvent(NULL, true, false, NULL);
    overlapThread = new Win_QextSerialThread(this);
    bytesToWriteLock = new QReadWriteLock;
}

/*!
\fn Win_QextSerialPort::~Win_QextSerialPort()
Standard destructor.
*/
Win_QextSerialPort::~Win_QextSerialPort() {
    if (isOpen()) {
        close();
    }
    CloseHandle(overlap.hEvent);
    delete overlapThread;
    delete bytesToWriteLock;
}

/*!
\fn Win_QextSerialPort& Win_QextSerialPort::operator=(const Win_QextSerialPort& s)
overrides the = operator
*/
Win_QextSerialPort& Win_QextSerialPort::operator=(const Win_QextSerialPort& s) {
    setOpenMode(s.openMode());
    _queryMode = s._queryMode;
    _bytesToWrite = s._bytesToWrite;
    bytesToWriteLock = new QReadWriteLock;
    overlapThread = new Win_QextSerialThread(this);
    memcpy(& overlap, & s.overlap, sizeof(OVERLAPPED));
    memcpy(& overlapWrite, & s.overlapWrite, sizeof(OVERLAPPED));
    lastErr=s.lastErr;
    port = s.port;
    Settings.FlowControl=s.Settings.FlowControl;
    Settings.Parity=s.Settings.Parity;
    Settings.DataBits=s.Settings.DataBits;
    Settings.StopBits=s.Settings.StopBits;
    Settings.BaudRate=s.Settings.BaudRate;
    Win_Handle=s.Win_Handle;
    memcpy(&Win_CommConfig, &s.Win_CommConfig, sizeof(COMMCONFIG));
    memcpy(&Win_CommTimeouts, &s.Win_CommTimeouts, sizeof(COMMTIMEOUTS));
    if (s.overlapThread->isRunning())
        overlapThread->start();
    return *this;
}


/*!
\fn bool Win_QextSerialPort::open(OpenMode mode)
Opens a serial port.  Note that this function does not specify which device to open.  If you need
to open a device by name, see Win_QextSerialPort::open(const char*).  This function has no effect
if the port associated with the class is already open.  The port is also configured to the current
settings, as stored in the Settings structure.
*/
bool Win_QextSerialPort::open(OpenMode mode) {
    unsigned long confSize = sizeof(COMMCONFIG);
    Win_CommConfig.dwSize = confSize;
    DWORD dwFlagsAndAttributes = 0;
    if (queryMode() == QextSerialBase::EventDriven)
        dwFlagsAndAttributes += FILE_FLAG_OVERLAPPED;

    LOCK_MUTEX();
    if (mode == QIODevice::NotOpen)
        return isOpen();
    if (!isOpen()) {
        /*open the port*/
        Win_Handle=CreateFileA(port.toLatin1(), GENERIC_READ|GENERIC_WRITE,
                              FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, dwFlagsAndAttributes, NULL);
        if (Win_Handle!=INVALID_HANDLE_VALUE) {
            /*configure port settings*/
            GetCommConfig(Win_Handle, &Win_CommConfig, &confSize);
            GetCommState(Win_Handle, &(Win_CommConfig.dcb));

            /*set up parameters*/
            Win_CommConfig.dcb.fBinary=TRUE;
            Win_CommConfig.dcb.fInX=FALSE;
            Win_CommConfig.dcb.fOutX=FALSE;
            Win_CommConfig.dcb.fAbortOnError=FALSE;
            Win_CommConfig.dcb.fNull=FALSE;
            setBaudRate(Settings.BaudRate);
            setDataBits(Settings.DataBits);
            setStopBits(Settings.StopBits);
            setParity(Settings.Parity);
            setFlowControl(Settings.FlowControl);
            setTimeout(Settings.Timeout_Millisec);
            SetCommConfig(Win_Handle, &Win_CommConfig, sizeof(COMMCONFIG));

            //init event driven approach
            if (queryMode() == QextSerialBase::EventDriven) {
                Win_CommTimeouts.ReadIntervalTimeout = MAXDWORD;
                Win_CommTimeouts.ReadTotalTimeoutMultiplier = 0;
                Win_CommTimeouts.ReadTotalTimeoutConstant = 0;
                Win_CommTimeouts.WriteTotalTimeoutMultiplier = 0;
                Win_CommTimeouts.WriteTotalTimeoutConstant = 0;
                SetCommTimeouts(Win_Handle, &Win_CommTimeouts);
                if (!SetCommMask( Win_Handle, EV_TXEMPTY | EV_RXCHAR | EV_DSR)) {
                    qWarning("Failed to set Comm Mask. Error code: %ld", GetLastError());
                    UNLOCK_MUTEX();
                    return false;
                }
                overlapThread->start();
            }
            QIODevice::open(mode);
        }
    } else {
        UNLOCK_MUTEX();
        return false;
    }
    UNLOCK_MUTEX();
    return isOpen();
}

/*!
\fn void Win_QextSerialPort::close()
Closes a serial port.  This function has no effect if the serial port associated with the class
is not currently open.
*/
void Win_QextSerialPort::close()
{
    LOCK_MUTEX();

    if (isOpen()) {
        flush();
        if (overlapThread->isRunning()) {
            overlapThread->stop();
            if (QThread::currentThread() != overlapThread)
                overlapThread->wait();
        }
        if (CloseHandle(Win_Handle))
            Win_Handle = INVALID_HANDLE_VALUE;
        _bytesToWrite = 0;
        QIODevice::close();
    }

    UNLOCK_MUTEX();
}

/*!
\fn void Win_QextSerialPort::flush()
Flushes all pending I/O to the serial port.  This function has no effect if the serial port
associated with the class is not currently open.
*/
void Win_QextSerialPort::flush() {
    LOCK_MUTEX();
    if (isOpen()) {
        FlushFileBuffers(Win_Handle);
    }
    UNLOCK_MUTEX();
}

/*!
\fn qint64 Win_QextSerialPort::size() const
This function will return the number of bytes waiting in the receive queue of the serial port.
It is included primarily to provide a complete QIODevice interface, and will not record errors
in the lastErr member (because it is const).  This function is also not thread-safe - in
multithreading situations, use Win_QextSerialPort::bytesAvailable() instead.
*/
qint64 Win_QextSerialPort::size() const {
    int availBytes;
    COMSTAT Win_ComStat;
    DWORD Win_ErrorMask=0;
    ClearCommError(Win_Handle, &Win_ErrorMask, &Win_ComStat);
    availBytes = Win_ComStat.cbInQue;
    return (qint64)availBytes;
}

/*!
\fn qint64 Win_QextSerialPort::bytesAvailable()
Returns the number of bytes waiting in the port's receive queue.  This function will return 0 if
the port is not currently open, or -1 on error.  Error information can be retrieved by calling
Win_QextSerialPort::getLastError().
*/
qint64 Win_QextSerialPort::bytesAvailable() {
    LOCK_MUTEX();
    if (isOpen()) {
        DWORD Errors;
        COMSTAT Status;
        bool success=ClearCommError(Win_Handle, &Errors, &Status);
        translateError(Errors);
        if (success) {
            lastErr=E_NO_ERROR;
            UNLOCK_MUTEX();
            return Status.cbInQue + QIODevice::bytesAvailable();
        }
        UNLOCK_MUTEX();
        return (unsigned int)-1;
    }
    UNLOCK_MUTEX();
    return 0;
}

/*!
\fn void Win_QextSerialPort::translateError(ulong error)
Translates a system-specific error code to a QextSerialPort error code.  Used internally.
*/
void Win_QextSerialPort::translateError(ulong error) {
    if (error&CE_BREAK) {
        lastErr=E_BREAK_CONDITION;
    }
    else if (error&CE_FRAME) {
        lastErr=E_FRAMING_ERROR;
    }
    else if (error&CE_IOE) {
        lastErr=E_IO_ERROR;
    }
    else if (error&CE_MODE) {
        lastErr=E_INVALID_FD;
    }
    else if (error&CE_OVERRUN) {
        lastErr=E_BUFFER_OVERRUN;
    }
    else if (error&CE_RXPARITY) {
        lastErr=E_RECEIVE_PARITY_ERROR;
    }
    else if (error&CE_RXOVER) {
        lastErr=E_RECEIVE_OVERFLOW;
    }
    else if (error&CE_TXFULL) {
        lastErr=E_TRANSMIT_OVERFLOW;
    }
}

/*!
\fn qint64 Win_QextSerialPort::readData(char *data, qint64 maxSize)
Reads a block of data from the serial port.  This function will read at most maxlen bytes from
the serial port and place them in the buffer pointed to by data.  Return value is the number of
bytes actually read, or -1 on error.

\warning before calling this function ensure that serial port associated with this class
is currently open (use isOpen() function to check if port is open).
*/
qint64 Win_QextSerialPort::readData(char *data, qint64 maxSize)
{
    DWORD retVal;

    LOCK_MUTEX();

    retVal = 0;
    if (queryMode() == QextSerialBase::EventDriven) {
        OVERLAPPED overlapRead;
        overlapRead.Internal = 0;
        overlapRead.InternalHigh = 0;
        overlapRead.Offset = 0;
        overlapRead.OffsetHigh = 0;
        overlapRead.hEvent = CreateEvent(NULL, true, false, NULL);
        if (!ReadFile(Win_Handle, (void*)data, (DWORD)maxSize, & retVal, & overlapRead)) {
            if (GetLastError() == ERROR_IO_PENDING)
                GetOverlappedResult(Win_Handle, & overlapRead, & retVal, true);
            else {
                lastErr = E_READ_FAILED;
                retVal = (DWORD)-1;
            }
        }
        CloseHandle(overlapRead.hEvent);
    } else if (!ReadFile(Win_Handle, (void*)data, (DWORD)maxSize, & retVal, NULL)) {
        lastErr = E_READ_FAILED;
        retVal = (DWORD)-1;
    }

    UNLOCK_MUTEX();

    return (qint64)retVal;
}

/*!
\fn qint64 Win_QextSerialPort::writeData(const char *data, qint64 maxSize)
Writes a block of data to the serial port.  This function will write len bytes
from the buffer pointed to by data to the serial port.  Return value is the number
of bytes actually written, or -1 on error.

\warning before calling this function ensure that serial port associated with this class
is currently open (use isOpen() function to check if port is open).
*/
qint64 Win_QextSerialPort::writeData(const char *data, qint64 maxSize)
{
    DWORD retVal;

    LOCK_MUTEX();

    retVal = 0;
    if (queryMode() == QextSerialBase::EventDriven) {
        bytesToWriteLock->lockForWrite();
        _bytesToWrite += maxSize;
        bytesToWriteLock->unlock();
        overlapWrite.Internal = 0;
        overlapWrite.InternalHigh = 0;
        overlapWrite.Offset = 0;
        overlapWrite.OffsetHigh = 0;
        overlapWrite.hEvent = CreateEvent(NULL, true, false, NULL);
        if (!WriteFile(Win_Handle, (void*)data, (DWORD)maxSize, & retVal, & overlapWrite)) {
            lastErr = E_WRITE_FAILED;
            retVal = (DWORD)-1;
        } else
            retVal = maxSize;
    } else if (!WriteFile(Win_Handle, (void*)data, (DWORD)maxSize, & retVal, NULL)) {
        lastErr = E_WRITE_FAILED;
        retVal = (DWORD)-1;
    }

    UNLOCK_MUTEX();

    return (qint64)retVal;
}

/*!
\fn void Win_QextSerialPort::ungetChar(char c)
This function is included to implement the full QIODevice interface, and currently has no
purpose within this class.  This function is meaningless on an unbuffered device and currently
only prints a warning message to that effect.
*/
void Win_QextSerialPort::ungetChar(char c) {

    /*meaningless on unbuffered sequential device - return error and print a warning*/
    TTY_WARNING("Win_QextSerialPort: ungetChar() called on an unbuffered sequential device - operation is meaningless");
}

/*!
\fn void Win_QextSerialPort::setFlowControl(FlowType flow)
Sets the flow control used by the port.  Possible values of flow are:
\verbatim
    FLOW_OFF            No flow control
    FLOW_HARDWARE       Hardware (RTS/CTS) flow control
    FLOW_XONXOFF        Software (XON/XOFF) flow control
\endverbatim
*/
void Win_QextSerialPort::setFlowControl(FlowType flow) {
    LOCK_MUTEX();
    if (Settings.FlowControl!=flow) {
        Settings.FlowControl=flow;
    }
    if (isOpen()) {
        switch(flow) {

            /*no flow control*/
            case FLOW_OFF:
                Win_CommConfig.dcb.fOutxCtsFlow=FALSE;
                Win_CommConfig.dcb.fRtsControl=RTS_CONTROL_DISABLE;
                Win_CommConfig.dcb.fInX=FALSE;
                Win_CommConfig.dcb.fOutX=FALSE;
                SetCommConfig(Win_Handle, &Win_CommConfig, sizeof(COMMCONFIG));
                break;

            /*software (XON/XOFF) flow control*/
            case FLOW_XONXOFF:
                Win_CommConfig.dcb.fOutxCtsFlow=FALSE;
                Win_CommConfig.dcb.fRtsControl=RTS_CONTROL_DISABLE;
                Win_CommConfig.dcb.fInX=TRUE;
                Win_CommConfig.dcb.fOutX=TRUE;
                SetCommConfig(Win_Handle, &Win_CommConfig, sizeof(COMMCONFIG));
                break;

            case FLOW_HARDWARE:
                Win_CommConfig.dcb.fOutxCtsFlow=TRUE;
                Win_CommConfig.dcb.fRtsControl=RTS_CONTROL_HANDSHAKE;
                Win_CommConfig.dcb.fInX=FALSE;
                Win_CommConfig.dcb.fOutX=FALSE;
                SetCommConfig(Win_Handle, &Win_CommConfig, sizeof(COMMCONFIG));
                break;
        }
    }
    UNLOCK_MUTEX();
}

/*!
\fn void Win_QextSerialPort::setParity(ParityType parity)
Sets the parity associated with the serial port.  The possible values of parity are:
\verbatim
    PAR_SPACE       Space Parity
    PAR_MARK        Mark Parity
    PAR_NONE        No Parity
    PAR_EVEN        Even Parity
    PAR_ODD         Odd Parity
\endverbatim
*/
void Win_QextSerialPort::setParity(ParityType parity) {
    LOCK_MUTEX();
    if (Settings.Parity!=parity) {
        Settings.Parity=parity;
    }
    if (isOpen()) {
        Win_CommConfig.dcb.Parity=(unsigned char)parity;
        switch (parity) {

            /*space parity*/
            case PAR_SPACE:
                if (Settings.DataBits==DATA_8) {
                    TTY_PORTABILITY_WARNING("Win_QextSerialPort Portability Warning: Space parity with 8 data bits is not supported by POSIX systems.");
                }
                Win_CommConfig.dcb.fParity=TRUE;
                break;

            /*mark parity - WINDOWS ONLY*/
            case PAR_MARK:
                TTY_PORTABILITY_WARNING("Win_QextSerialPort Portability Warning:  Mark parity is not supported by POSIX systems");
                Win_CommConfig.dcb.fParity=TRUE;
                break;

            /*no parity*/
            case PAR_NONE:
                Win_CommConfig.dcb.fParity=FALSE;
                break;

            /*even parity*/
            case PAR_EVEN:
                Win_CommConfig.dcb.fParity=TRUE;
                break;

            /*odd parity*/
            case PAR_ODD:
                Win_CommConfig.dcb.fParity=TRUE;
                break;
        }
        SetCommConfig(Win_Handle, &Win_CommConfig, sizeof(COMMCONFIG));
    }
    UNLOCK_MUTEX();
}

/*!
\fn void Win_QextSerialPort::setDataBits(DataBitsType dataBits)
Sets the number of data bits used by the serial port.  Possible values of dataBits are:
\verbatim
    DATA_5      5 data bits
    DATA_6      6 data bits
    DATA_7      7 data bits
    DATA_8      8 data bits
\endverbatim

\note
This function is subject to the following restrictions:
\par
    5 data bits cannot be used with 2 stop bits.
\par
    1.5 stop bits can only be used with 5 data bits.
\par
    8 data bits cannot be used with space parity on POSIX systems.

*/
void Win_QextSerialPort::setDataBits(DataBitsType dataBits) {
    LOCK_MUTEX();
    if (Settings.DataBits!=dataBits) {
        if ((Settings.StopBits==STOP_2 && dataBits==DATA_5) ||
            (Settings.StopBits==STOP_1_5 && dataBits!=DATA_5)) {
        }
        else {
            Settings.DataBits=dataBits;
        }
    }
    if (isOpen()) {
        switch(dataBits) {

            /*5 data bits*/
            case DATA_5:
                if (Settings.StopBits==STOP_2) {
                    TTY_WARNING("Win_QextSerialPort: 5 Data bits cannot be used with 2 stop bits.");
                }
                else {
                    Win_CommConfig.dcb.ByteSize=5;
                    SetCommConfig(Win_Handle, &Win_CommConfig, sizeof(COMMCONFIG));
                }
                break;

            /*6 data bits*/
            case DATA_6:
                if (Settings.StopBits==STOP_1_5) {
                    TTY_WARNING("Win_QextSerialPort: 6 Data bits cannot be used with 1.5 stop bits.");
                }
                else {
                    Win_CommConfig.dcb.ByteSize=6;
                    SetCommConfig(Win_Handle, &Win_CommConfig, sizeof(COMMCONFIG));
                }
                break;

            /*7 data bits*/
            case DATA_7:
                if (Settings.StopBits==STOP_1_5) {
                    TTY_WARNING("Win_QextSerialPort: 7 Data bits cannot be used with 1.5 stop bits.");
                }
                else {
                    Win_CommConfig.dcb.ByteSize=7;
                    SetCommConfig(Win_Handle, &Win_CommConfig, sizeof(COMMCONFIG));
                }
                break;

            /*8 data bits*/
            case DATA_8:
                if (Settings.StopBits==STOP_1_5) {
                    TTY_WARNING("Win_QextSerialPort: 8 Data bits cannot be used with 1.5 stop bits.");
                }
                else {
                    Win_CommConfig.dcb.ByteSize=8;
                    SetCommConfig(Win_Handle, &Win_CommConfig, sizeof(COMMCONFIG));
                }
                break;
        }
    }
    UNLOCK_MUTEX();
}

/*!
\fn void Win_QextSerialPort::setStopBits(StopBitsType stopBits)
Sets the number of stop bits used by the serial port.  Possible values of stopBits are:
\verbatim
    STOP_1      1 stop bit
    STOP_1_5    1.5 stop bits
    STOP_2      2 stop bits
\endverbatim

\note
This function is subject to the following restrictions:
\par
    2 stop bits cannot be used with 5 data bits.
\par
    1.5 stop bits cannot be used with 6 or more data bits.
\par
    POSIX does not support 1.5 stop bits.
*/
void Win_QextSerialPort::setStopBits(StopBitsType stopBits) {
    LOCK_MUTEX();
    if (Settings.StopBits!=stopBits) {
        if ((Settings.DataBits==DATA_5 && stopBits==STOP_2) ||
            (stopBits==STOP_1_5 && Settings.DataBits!=DATA_5)) {
        }
        else {
            Settings.StopBits=stopBits;
        }
    }
    if (isOpen()) {
        switch (stopBits) {

            /*one stop bit*/
            case STOP_1:
                Win_CommConfig.dcb.StopBits=ONESTOPBIT;
                SetCommConfig(Win_Handle, &Win_CommConfig, sizeof(COMMCONFIG));
                break;

            /*1.5 stop bits*/
            case STOP_1_5:
                TTY_PORTABILITY_WARNING("Win_QextSerialPort Portability Warning: 1.5 stop bit operation is not supported by POSIX.");
                if (Settings.DataBits!=DATA_5) {
                    TTY_WARNING("Win_QextSerialPort: 1.5 stop bits can only be used with 5 data bits");
                }
                else {
                    Win_CommConfig.dcb.StopBits=ONE5STOPBITS;
                    SetCommConfig(Win_Handle, &Win_CommConfig, sizeof(COMMCONFIG));
                }
                break;

            /*two stop bits*/
            case STOP_2:
                if (Settings.DataBits==DATA_5) {
                    TTY_WARNING("Win_QextSerialPort: 2 stop bits cannot be used with 5 data bits");
                }
                else {
                    Win_CommConfig.dcb.StopBits=TWOSTOPBITS;
                    SetCommConfig(Win_Handle, &Win_CommConfig, sizeof(COMMCONFIG));
                }
                break;
        }
    }
    UNLOCK_MUTEX();
}

/*!
\fn void Win_QextSerialPort::setBaudRate(BaudRateType baudRate)
Sets the baud rate of the serial port.  Note that not all rates are applicable on
all platforms.  The following table shows translations of the various baud rate
constants on Windows(including NT/2000) and POSIX platforms.  Speeds marked with an *
are speeds that are usable on both Windows and POSIX.
\verbatim

  RATE          Windows Speed   POSIX Speed
  -----------   -------------   -----------
   BAUD50                 110          50
   BAUD75                 110          75
  *BAUD110                110         110
   BAUD134                110         134.5
   BAUD150                110         150
   BAUD200                110         200
  *BAUD300                300         300
  *BAUD600                600         600
  *BAUD1200              1200        1200
   BAUD1800              1200        1800
  *BAUD2400              2400        2400
  *BAUD4800              4800        4800
  *BAUD9600              9600        9600
   BAUD14400            14400        9600
  *BAUD19200            19200       19200
  *BAUD38400            38400       38400
   BAUD56000            56000       38400
  *BAUD57600            57600       57600
   BAUD76800            57600       76800
  *BAUD115200          115200      115200
   BAUD128000          128000      115200
   BAUD256000          256000      115200
\endverbatim
*/
void Win_QextSerialPort::setBaudRate(BaudRateType baudRate) {
    LOCK_MUTEX();
    if (Settings.BaudRate!=baudRate) {
        switch (baudRate) {
            case BAUD50:
            case BAUD75:
            case BAUD134:
            case BAUD150:
            case BAUD200:
                Settings.BaudRate=BAUD110;
                break;

            case BAUD1800:
                Settings.BaudRate=BAUD1200;
                break;

            case BAUD76800:
                Settings.BaudRate=BAUD57600;
                break;

            default:
                Settings.BaudRate=baudRate;
                break;
        }
    }
    if (isOpen()) {
        switch (baudRate) {

            /*50 baud*/
            case BAUD50:
                TTY_WARNING("Win_QextSerialPort: Windows does not support 50 baud operation.  Switching to 110 baud.");
                Win_CommConfig.dcb.BaudRate=CBR_110;
                break;

            /*75 baud*/
            case BAUD75:
                TTY_WARNING("Win_QextSerialPort: Windows does not support 75 baud operation.  Switching to 110 baud.");
                Win_CommConfig.dcb.BaudRate=CBR_110;
                break;

            /*110 baud*/
            case BAUD110:
                Win_CommConfig.dcb.BaudRate=CBR_110;
                break;

            /*134.5 baud*/
            case BAUD134:
                TTY_WARNING("Win_QextSerialPort: Windows does not support 134.5 baud operation.  Switching to 110 baud.");
                Win_CommConfig.dcb.BaudRate=CBR_110;
                break;

            /*150 baud*/
            case BAUD150:
                TTY_WARNING("Win_QextSerialPort: Windows does not support 150 baud operation.  Switching to 110 baud.");
                Win_CommConfig.dcb.BaudRate=CBR_110;
                break;

            /*200 baud*/
            case BAUD200:
                TTY_WARNING("Win_QextSerialPort: Windows does not support 200 baud operation.  Switching to 110 baud.");
                Win_CommConfig.dcb.BaudRate=CBR_110;
                break;

            /*300 baud*/
            case BAUD300:
                Win_CommConfig.dcb.BaudRate=CBR_300;
                break;

            /*600 baud*/
            case BAUD600:
                Win_CommConfig.dcb.BaudRate=CBR_600;
                break;

            /*1200 baud*/
            case BAUD1200:
                Win_CommConfig.dcb.BaudRate=CBR_1200;
                break;

            /*1800 baud*/
            case BAUD1800:
                TTY_WARNING("Win_QextSerialPort: Windows does not support 1800 baud operation.  Switching to 1200 baud.");
                Win_CommConfig.dcb.BaudRate=CBR_1200;
                break;

            /*2400 baud*/
            case BAUD2400:
                Win_CommConfig.dcb.BaudRate=CBR_2400;
                break;

            /*4800 baud*/
            case BAUD4800:
                Win_CommConfig.dcb.BaudRate=CBR_4800;
                break;

            /*9600 baud*/
            case BAUD9600:
                Win_CommConfig.dcb.BaudRate=CBR_9600;
                break;

            /*14400 baud*/
            case BAUD14400:
                TTY_PORTABILITY_WARNING("Win_QextSerialPort Portability Warning: POSIX does not support 14400 baud operation.");
                Win_CommConfig.dcb.BaudRate=CBR_14400;
                break;

            /*19200 baud*/
            case BAUD19200:
                Win_CommConfig.dcb.BaudRate=CBR_19200;
                break;

            /*38400 baud*/
            case BAUD38400:
                Win_CommConfig.dcb.BaudRate=CBR_38400;
                break;

            /*56000 baud*/
            case BAUD56000:
                TTY_PORTABILITY_WARNING("Win_QextSerialPort Portability Warning: POSIX does not support 56000 baud operation.");
                Win_CommConfig.dcb.BaudRate=CBR_56000;
                break;

            /*57600 baud*/
            case BAUD57600:
                Win_CommConfig.dcb.BaudRate=CBR_57600;
                break;

            /*76800 baud*/
            case BAUD76800:
                TTY_WARNING("Win_QextSerialPort: Windows does not support 76800 baud operation.  Switching to 57600 baud.");
                Win_CommConfig.dcb.BaudRate=CBR_57600;
                break;

            /*115200 baud*/
            case BAUD115200:
                Win_CommConfig.dcb.BaudRate=CBR_115200;
                break;

            /*128000 baud*/
            case BAUD128000:
                TTY_PORTABILITY_WARNING("Win_QextSerialPort Portability Warning: POSIX does not support 128000 baud operation.");
                Win_CommConfig.dcb.BaudRate=CBR_128000;
                break;

            /*256000 baud*/
            case BAUD256000:
                TTY_PORTABILITY_WARNING("Win_QextSerialPort Portability Warning: POSIX does not support 256000 baud operation.");
                Win_CommConfig.dcb.BaudRate=CBR_256000;
                break;
        }
        SetCommConfig(Win_Handle, &Win_CommConfig, sizeof(COMMCONFIG));
    }
    UNLOCK_MUTEX();
}

/*!
\fn void Win_QextSerialPort::setDtr(bool set)
Sets DTR line to the requested state (high by default).  This function will have no effect if
the port associated with the class is not currently open.
*/
void Win_QextSerialPort::setDtr(bool set) {
    LOCK_MUTEX();
    if (isOpen()) {
        if (set) {
            EscapeCommFunction(Win_Handle, SETDTR);
        }
        else {
            EscapeCommFunction(Win_Handle, CLRDTR);
        }
    }
    UNLOCK_MUTEX();
}

/*!
\fn void Win_QextSerialPort::setRts(bool set)
Sets RTS line to the requested state (high by default).  This function will have no effect if
the port associated with the class is not currently open.
*/
void Win_QextSerialPort::setRts(bool set) {
    LOCK_MUTEX();
    if (isOpen()) {
        if (set) {
            EscapeCommFunction(Win_Handle, SETRTS);
        }
        else {
            EscapeCommFunction(Win_Handle, CLRRTS);
        }
    }
    UNLOCK_MUTEX();
}

/*!
\fn ulong Win_QextSerialPort::lineStatus(void)
returns the line status as stored by the port function.  This function will retrieve the states
of the following lines: DCD, CTS, DSR, and RI.  On POSIX systems, the following additional lines
can be monitored: DTR, RTS, Secondary TXD, and Secondary RXD.  The value returned is an unsigned
long with specific bits indicating which lines are high.  The following constants should be used
to examine the states of individual lines:

\verbatim
Mask        Line
------      ----
LS_CTS      CTS
LS_DSR      DSR
LS_DCD      DCD
LS_RI       RI
\endverbatim

This function will return 0 if the port associated with the class is not currently open.
*/
ulong Win_QextSerialPort::lineStatus(void) {
    unsigned long Status=0, Temp=0;
    LOCK_MUTEX();
    if (isOpen()) {
        GetCommModemStatus(Win_Handle, &Temp);
        if (Temp&MS_CTS_ON) {
            Status|=LS_CTS;
        }
        if (Temp&MS_DSR_ON) {
            Status|=LS_DSR;
        }
        if (Temp&MS_RING_ON) {
            Status|=LS_RI;
        }
        if (Temp&MS_RLSD_ON) {
            Status|=LS_DCD;
        }
    }
    UNLOCK_MUTEX();
    return Status;
}

bool Win_QextSerialPort::waitForReadyRead(int msecs)
{
    //@todo implement
    return false;
}

qint64 Win_QextSerialPort::bytesToWrite() const
{
    return _bytesToWrite;
}

void Win_QextSerialPort::monitorCommEvent()
{
    DWORD eventMask = 0;

    ResetEvent(overlap.hEvent);
    if (!WaitCommEvent(Win_Handle, & eventMask, & overlap))
        if (GetLastError() != ERROR_IO_PENDING)
            qCritical("WaitCommEvent error %ld\n", GetLastError());

    if (WaitForSingleObject(overlap.hEvent, INFINITE) == WAIT_OBJECT_0) {
        //overlap event occured
        DWORD undefined;
        if (!GetOverlappedResult(Win_Handle, & overlap, & undefined, false)) {
            qWarning("Comm event overlapped error %ld", GetLastError());
            return;
        }
        if (eventMask & EV_RXCHAR) {
            if (sender() != this)
                emit readyRead();
        }
        if (eventMask & EV_TXEMPTY) {
            DWORD numBytes;
            GetOverlappedResult(Win_Handle, & overlapWrite, & numBytes, true);
            bytesToWriteLock->lockForWrite();
            if (sender() != this)
                emit bytesWritten(bytesToWrite());
            _bytesToWrite = 0;
            bytesToWriteLock->unlock();
        }
        if (eventMask & EV_DSR)
            if (lineStatus() & LS_DSR)
                emit dsrChanged(true);
            else
                emit dsrChanged(false);
    }
}

void Win_QextSerialPort::terminateCommWait()
{
    SetCommMask(Win_Handle, 0);
}


/*!
\fn void Win_QextSerialPort::setTimeout(ulong millisec);
Sets the read and write timeouts for the port to millisec milliseconds.
Setting 0 for both sec and millisec indicates that timeouts are not used for read nor
write operations. Setting -1 indicates that read and write should return immediately.

\note this function does nothing in event driven mode.
*/
void Win_QextSerialPort::setTimeout(long millisec) {
    LOCK_MUTEX();
    Settings.Timeout_Millisec = millisec;

    if (millisec == -1) {
        Win_CommTimeouts.ReadIntervalTimeout = MAXDWORD;
        Win_CommTimeouts.ReadTotalTimeoutConstant = 0;
    } else {
        Win_CommTimeouts.ReadIntervalTimeout = millisec;
        Win_CommTimeouts.ReadTotalTimeoutConstant = millisec;
    }
    Win_CommTimeouts.ReadTotalTimeoutMultiplier = 0;
    Win_CommTimeouts.WriteTotalTimeoutMultiplier = millisec;
    Win_CommTimeouts.WriteTotalTimeoutConstant = 0;
    if (queryMode() != QextSerialBase::EventDriven)
        SetCommTimeouts(Win_Handle, &Win_CommTimeouts);

    UNLOCK_MUTEX();
}


Win_QextSerialThread::Win_QextSerialThread(Win_QextSerialPort * qesp):
    QThread()
{
    this->qesp = qesp;
    terminate = false;
}

void Win_QextSerialThread::stop()
{
    terminate = true;
    qesp->terminateCommWait();
}

void Win_QextSerialThread::run()
{
    while (!terminate)
        qesp->monitorCommEvent();
    terminate = false;
}














