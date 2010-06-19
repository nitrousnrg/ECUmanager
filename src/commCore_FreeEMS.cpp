#include "commCore_FreeEMS.h"

aPacket::aPacket()
{
    payloadLength = 0;
    headerFlags = 0;
    fullPacket = "";
}

void aPacket::setPacket(QByteArray newPacket)
{
    union intByte
    {
        BYTE byte[2];
        int entero;
    } bytesID;
    fullPacket = newPacket;
//    unpreparePacket();
    headerFlags = fullPacket[0];
    bytesID.byte[1] = fullPacket[1];
    bytesID.byte[0] = fullPacket[2];

    payloadID = fullPacket[1]*256+fullPacket[2];
    //payloadID = bytesID.entero;
qDebug("fullPacket[0] = %d",headerFlags);
qDebug("fullPacket[1] = %d",bytesID.byte[1]);
qDebug("fullPacket[2] = %d",bytesID.byte[0]);
qDebug("payloadID = %d",payloadID);
}

QByteArray aPacket::getPacket()
{
    return fullPacket;
}


void aPacket::setHeaderAckFlag(bool bit)
{
    if(bit)
        headerFlags |= 0x40;
    else
        headerFlags &= ~0x40;
}
bool aPacket::hasHeaderAckFlag()
{
    return headerFlags &= 0x40;
}

void aPacket::setHeaderLengthFlag(bool bit)
{
    if(bit)
        headerFlags |= 0x10;
    else
        headerFlags &= ~0x10;
}

bool aPacket::hasHeaderLengthFlag()
{
    return headerFlags &= 0x10;
}

int aPacket::getPayloadID()
{
    payloadID = fullPacket[1]*256 + fullPacket[2];      //2nd and 3rd bytes correspond to the payloadID *always*
    return payloadID;
}

void aPacket::setPayloadID(int newID)
{
    payloadID = newID;
}

void aPacket::setPayload(QByteArray newPayload)
{
    payload = newPayload;
}
QByteArray aPacket::getPayload()
{
    return payload;
}

void aPacket::setPayloadLength(int newLength)
{
    payloadLength = newLength;
}

int aPacket::getPayloadLength()
{
    if( hasHeaderLengthFlag() )     //the flag must be set to extract the length
    {
        int n = 4;
        if( hasHeaderAckFlag() )    //check for optional bytes
            ++n;
        payloadLength = fullPacket[n]*256 + fullPacket[n+1];
        return payloadLength;
    }
    else
        return 0;
}

int aPacket::getCalculatedPayloadLength()
{
    return payload.size();
}

void aPacket::buildPacket()
{
    fullPacket[0] = headerFlags;
    fullPacket.append(payloadID/256);  //high byte
    fullPacket.append(payloadID);      //low byte
    fullPacket.append(payload);

    unsigned char checksum = 0;
    for(int i=0; i < fullPacket.size(); ++i)
        checksum ^= fullPacket[i];
    fullPacket.append(checksum);
    qDebug(fullPacket.data());
}

void aPacket::preparePacket()       //to send trough RS232 we need to add start/stop bytes
{
    /* add escaping scheme */
    char startPair[2];
    char escapePair[2];
    char endPair[2];

    startPair[0] = 0xBB;
    startPair[1] = 0x55;
    escapePair[0] = 0xBB;
    escapePair[1] = 0x44;
    endPair[0] = 0xBB;
    endPair[1] = 0x33;

    fullPacket.replace(0xAA, startPair);        //replace start byte with the respective start pair
    fullPacket.replace(0xBB, escapePair);
    fullPacket.replace(0xCC, endPair);

    fullPacket.prepend(0xAA);       //prepend a start byte
    fullPacket.append(0xCC);        //append an end byte

    /* the packet is now ready to send */

}

void aPacket::unpreparePacket()
{
       /* remove escaping scheme */

    QByteArray escapedStart;
    escapedStart[0] = 0xBB;
    escapedStart[1] = 0x55;
    QByteArray start;
    start[0] = 0xAA;

    QByteArray escapedEscape;
    escapedEscape[0] = 0xBB;
    escapedEscape[1] = 0x44;
    QByteArray escape;
    escape[0] = 0xBB;

    QByteArray escapedEnd;
    escapedEnd[0] = 0xBB;
    escapedEnd[1] = 0x33;
    QByteArray end;
    end[0] = 0xCC;


    fullPacket.replace(escapedStart, start);
    fullPacket.replace(escapedEscape, escape);
    fullPacket.replace(escapedEnd, end);

    fullPacket= fullPacket.right(fullPacket.size()-1);      //remove start byte (0xAA = start)
    fullPacket.chop(1);                           //remove last byte (0xCC = end)
}


/*unsigned char aPacket::checksum(QByteArray data)
{
    unsigned char sum = 0;
    for (int i=0; i < data.size(); ++i)
        sum ^= data[i];
    return sum;
}
*/


void qt4application::sendReset()
{
    qDebug("emited!!!");
    aPacket packet;
    packet.setHeaderAckFlag(false);
    packet.setHeaderLengthFlag(true);  //false
    packet.setPayloadID(requestSoftReset);
    packet.buildPacket();
    packet.preparePacket();
    //serial->write(packet.getPacket(),1);
    qDebug(packet.getPacket().data());

    showPacket(packet.getPacket());
}

void commThread::getInterfaceVersion()
{
    aPacket packet;
    packet.setHeaderAckFlag(false);
    packet.setHeaderLengthFlag(false);
    packet.setPayloadID(requestInterfaceVersion);
    packet.buildPacket();
    packet.preparePacket();
   // serial->write(packet.getPacket(),1);
    emit packetArrived(packet.getPacket());

}

void commThread::getFirmwareVersion()
{
    aPacket packet;
    packet.setHeaderAckFlag(false);
    packet.setHeaderLengthFlag(false);
    packet.setPayloadID(requestFirmwareVersion);
    packet.buildPacket();
    packet.preparePacket();
    //serial->write(packet.getPacket(),1);
    emit packetArrived(packet.getPacket());
}


/* this is where every packet is extracted from the incoming stream, and parsed to be used later */
void commThread::readFreeEMS()
{
    const qint64 available = serial->bytesAvailable();
    buffer.append(serial->read(available));

    if( buffer.contains(0xCC) )     //there is an end char in the buffer (we have a full packet here!)
    {
        int start = buffer.indexOf(0xAA);    //this is where the end of my packet is located
        int end = buffer.indexOf(0xCC);    //this is where the end of my packet is located
        QByteArray auxBuffer = buffer;
        auxBuffer.remove(0, start);                     //remove first chunk
        auxBuffer.remove(end, auxBuffer.size()-1);      //remove last chunk

        aPacket received_packet;
        received_packet.setPacket(auxBuffer);

        emit packetArrived(auxBuffer);
    }


}

void qt4application::showPacket(QByteArray dataArray)
{
    aPacket data;

    data.setPacket(dataArray);
    qDebug(data.getPacket().data());
    data.unpreparePacket();
    qDebug(data.getPacket().data());

    qDebug("nooo");
    if(debugEnabledFlag)
    {
        QTime time = QTime::currentTime();
        QTableWidgetItem *newItem = new QTableWidgetItem(time.toString("hh:mm:ss.zzz"));
        packetTable->setItem(0,0,newItem );

        QString headerFlags;
        if( data.hasHeaderAckFlag() )
            headerFlags.append("A ");
        else
            headerFlags.append("- ");
        if( data.hasHeaderLengthFlag() )
            headerFlags.append("F");
        else
            headerFlags.append("- ");

        QTableWidgetItem *newItem2 = new QTableWidgetItem(headerFlags);
        packetTable->setItem(0,1,newItem2 );
        QTableWidgetItem *newItem3 = new QTableWidgetItem(data.getPayloadID());
        packetTable->setItem(0,2,newItem3 );
        QTableWidgetItem *newItem4 = new QTableWidgetItem(data.getPayload().constData());
        packetTable->setItem(0,3,newItem4 );
    }
}

void qt4application::debugFreeEMS()
{
    if( serialThread )
    {
        dialog = new QDialog(this);
        dialog->setModal(false);
        nitrousDialogOpen = true;
        dialog->setSizeGripEnabled(false);
        dialog->resize(728,560);
        dialog->setWindowTitle(tr("Debug"));

        debugEnabledFlag = true;
        
        QGridLayout *grid = new QGridLayout;
        QDialogButtonBox *accept = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(accept, SIGNAL(accepted()), this, SLOT(acceptDialog()));
        connect(accept, SIGNAL(rejected()), this, SLOT(rejectDialog()));

        QLabel *flow1 = new QLabel(tr("Extra fuel (ms)"));
        QLabel *retard1 = new QLabel(tr("Ignition retard"));

        QLabel *flow2 = new QLabel(tr("Extra fuel (ms)"));
        QLabel *retard2 = new QLabel(tr("Ignition retard"));

        /*  conditions  */
        editNitrousRPM = new QLineEdit(string1.setNum(confParameter.nitrousRPM));
        editNitrousMAP = new QLineEdit(string1.setNum(confParameter.nitrousMAP));
        editNitrousTPS = new QLineEdit(string1.setNum(confParameter.nitrousTPS));

        editNitrousFlow1 = new QLineEdit(string1.setNum(confParameter.nitrousStage1Flow));
        editNitrousFlow2 = new QLineEdit(string1.setNum(confParameter.nitrousStage2Flow));
        editNitrousRetard1 = new QLineEdit(string1.setNum(confParameter.nitrousStage1Retard));
        editNitrousRetard2 = new QLineEdit(string1.setNum(confParameter.nitrousStage2Retard));

        editNitrousRPM->setAlignment(Qt::AlignRight);
        editNitrousMAP->setAlignment(Qt::AlignRight);
        editNitrousTPS->setAlignment(Qt::AlignRight);
        editNitrousFlow1->setAlignment(Qt::AlignRight);
        editNitrousFlow2->setAlignment(Qt::AlignRight);
        editNitrousRetard1->setAlignment(Qt::AlignRight);
        editNitrousRetard2->setAlignment(Qt::AlignRight);

       /* table that holds every packet send/received */

        packetTable= new QTableWidget(1, 4, this);

        QStringList headerLabels;
        headerLabels << "Time" << "Header" << "PayloadID"<<"Payload";
        packetTable->setHorizontalHeaderLabels(headerLabels);

        QGridLayout *vbox1 = new QGridLayout;
        vbox1->addWidget(packetTable,0,1);
   //     vbox1->setRowStretch(1,100);

        /*  stage 1  */
        QGridLayout *vbox2 = new QGridLayout;
        vbox2->addWidget(flow1,0,0);
        vbox2->addWidget(editNitrousFlow1,0,1);
        vbox2->addWidget(retard1,1,0);
        vbox2->addWidget(editNitrousRetard1,1,1);

        QTabWidget *tabBar = new QTabWidget();
        QWidget *conditions = new QWidget();
        conditions->setLayout(vbox1);
        QWidget *stage1 = new QWidget();
        stage1->setLayout(vbox2);
        tabBar->addTab(conditions,"Packets");
        tabBar->addTab(stage1,"Tests");

        //tabBar->setLayout(vbox);

        grid->addWidget(tabBar,1,0,1,2);
        grid->addWidget(accept,3,1);
        dialog->setLayout(grid);
        dialog->show();
    }
}








