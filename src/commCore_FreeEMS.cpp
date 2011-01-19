#include "commCore_FreeEMS.h"

/* This file contains mostly the tools used to decode and process
   the packets coming to and from FreeEMS */

aPacket::aPacket()
{
	payloadLength = 0;
	headerFlags = 0;
	fullPacket = "";
}


int aPacket::setPacket(QByteArray newPacket)
{
	fullPacket = newPacket;

	if(int error = removeEscape())
		return error;
	if(int error = check() )
		return error;

	headerFlags = fullPacket[0];
	payloadID = (unsigned char)fullPacket[1]*256 + (unsigned char)fullPacket.at(2);

	unsigned int index = 3;
	if( headerFlags & HEADER_HAS_LENGTH )
	{
		payloadLength = (unsigned char)fullPacket[index]*256 + (unsigned char)fullPacket[index+1];
		index += 2;
	}
	payload = fullPacket.mid(index,payloadLength);
	if((unsigned int)fullPacket.size() != (index + payloadLength + 1))
		return payload_size_inconsistency_error;

	//success!
	return 0;
}

QByteArray aPacket::getPacket()
{	return fullPacket;	}


void aPacket::setHeaderLengthFlag(bool bit)
{
	if(bit)
                headerFlags |= HEADER_HAS_LENGTH;
	else
                headerFlags &= ~HEADER_HAS_LENGTH;
}


bool aPacket::hasHeaderLengthFlag()
{	return headerFlags &= HEADER_HAS_LENGTH;	}


unsigned int aPacket::getPayloadID()
{	return payloadID;	}


void aPacket::setPayloadID(unsigned int newID)
{	payloadID = newID;	}


void aPacket::setPayload(QByteArray newPayload)
{	payload = newPayload;	}


QByteArray aPacket::getPayload()
{	return payload;	}


void aPacket::setPayloadLength(int newLength)
{	payloadLength = newLength;	}


unsigned int aPacket::getPayloadLength()
{
	if( hasHeaderLengthFlag() )	 //flag must be set to extract the length
	{
		int n = 4;
		payloadLength = fullPacket[n]*256 + fullPacket[n+1];
		return payloadLength;
	}
	else
		return 0;
}


int aPacket::getCalculatedPayloadLength()
{	return payload.size();	}


void aPacket::buildPacket()
{
	fullPacket[0] = headerFlags;
								 //high byte
	fullPacket.append(payloadID/256);
	fullPacket.append(payloadID);//low byte
	fullPacket.append(payload);

	unsigned char checksum = 0;
	for(int i=0; i < fullPacket.size(); ++i)
		checksum ^= fullPacket[i];
	fullPacket.append(checksum);
	qDebug(fullPacket.data());
}


void aPacket::addEscape()	 //to send trough RS232 we need to add start/stop bytes
{
	/* add escaping scheme */
	const char escapedStart[2]  = {0xBB,0x55};
	const char escapedEscape[2] = {0xBB,0x44};
	const char escapedEnd[2] = {0xBB,0x33};
	const char start  = 0xAA;
	const char escape = 0xBB;
	const char end = 0xCC;

	/*replace start byte with the respective start pair*/
	fullPacket.replace(start, escapedStart);
	fullPacket.replace(escape, escapedEscape);
	fullPacket.replace(end, escapedEnd);

	/* Add start and end bytes to the stream */
	fullPacket.prepend(0xAA);
	fullPacket.append(0xCC);

	/* the packet is now ready to send */
}


int aPacket::removeEscape()
{
	const char escapedStart[2]  = {0xBB,0x55};
	const char escapedEscape[2] = {0xBB,0x44};
	const char escapedEnd[2] = {0xBB,0x33};
	const char start  = 0xAA;
	const char escape = 0xBB;
	const char end = 0xCC;

	/* Check if it is a valid scaping scheme. It must have both start and end chars */
	if((fullPacket.count(start) ) != 1)
		return start_byte_inconsistency_error;
	if((fullPacket.count(end) ) != 1)
		return stop_byte_inconsistency_error;
	/* 0xBB can only be followed by 0x55,0x44 or 0x33, but that error is detected in the checksum too*/
	/*while(index = fullPacket.indexOf(escape) != -1)
	{
		char byte = fullPacket.at(index);
		if( (byte == 0x55)	&& (byte == 0x44) && (byte == 0x33))
			return;
	}*/

	/* Replace the escaped byte pairs with the original bytes */
	fullPacket.replace(escapedStart, &start);
	fullPacket.replace(escapedEscape, &escape);
	fullPacket.replace(escapedEnd, &end);

	/* Remove start and end bytes */
	fullPacket= fullPacket.right(fullPacket.size()-1);
	fullPacket.chop(1);
	
	return 0;
}


/*unsigned char aPacket::checksum(QByteArray data)
{
	unsigned char sum = 0;
	for (int i=0; i < data.size(); ++i)
		sum ^= data[i];
	return sum;
}
*/


/* If the checksum, payload length and escaping characters make sense, the packet is correct and check() returns true
   otherwise, the packet is corrupted and check returns false */
int aPacket::check()
{
	/*calculate checksum: it includes the header, adrress/length, payloadID and payload.
	 i.e., the fullPacket but the last byte	 */
	unsigned char sum=0;
	int i;
	for(i=0; i < (fullPacket.size() -1); ++i)
		sum += fullPacket.at(i);

	if(sum == (unsigned char)fullPacket.at(i))
		return 0;		/*Correct checksum*/
	else
	{
		qDebug("bad checksum:\ncalculated: %x\tgot:%x",sum,fullPacket.at(i));
		return bad_checksum_error;		/*Bad Checksum*/
	}
}

void aPacket::setChecksum()
{
	unsigned char sum = 0;
	for( int i=0; i<fullPacket.size(); i++ )
		sum += (unsigned char)fullPacket[i];
	fullPacket.append(sum);
}

void qt4application::sendReset()
{
	qDebug("Send Soft System Reset");
	aPacket packet;
	packet.setHeaderLengthFlag(true);
	packet.setPayloadID(requestSoftSystemReset);
	packet.buildPacket();
	packet.addEscape();
	//serial->write(packet.getPacket(),1);
	qDebug(packet.getPacket().data());

	showPacket(packet.getPacket());
}


void commThread::getInterfaceVersion()
{
	aPacket packet;
	packet.setHeaderLengthFlag(false);
	packet.setPayloadID(requestInterfaceVersion);
	packet.buildPacket();
	packet.addEscape();
	// serial->write(packet.getPacket(),1);
	emit packetArrived(packet.getPacket());
}


void commThread::getFirmwareVersion()
{
	aPacket packet;
	packet.setHeaderLengthFlag(false);
	packet.setPayloadID(requestFirmwareVersion);
	packet.buildPacket();
	packet.addEscape();
	//serial->write(packet.getPacket(),1);
	emit packetArrived(packet.getPacket());
}

void commThread::adjustMainTable()
{
}

/* This will be moved inside qt4application, and the packet decoding has to be
	done only once, in decodeFreeEMSPacket() */
void qt4application::showPacket(QByteArray dataArray)
{
	aPacket data;

	data.setPacket(dataArray);
	qDebug(data.getPacket().data());
	data.removeEscape();
	qDebug(data.getPacket().data());

	if(debugEnabledFlag)
	{
		QTime time = QTime::currentTime();
		QTableWidgetItem *newItem = new QTableWidgetItem(time.toString("hh:mm:ss.zzz"));
		packetTable->setItem(0,0,newItem );

		QString headerFlags;
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
		dialog->setSizeGripEnabled(false);
		dialog->resize(728,560);
		dialog->setWindowTitle(tr("Debug"));

		debugEnabledFlag = true;

		QGridLayout *grid = new QGridLayout;
		QDialogButtonBox *accept = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
		connect(accept, SIGNAL(accepted()), this, SLOT(acceptDialog()));
		connect(accept, SIGNAL(rejected()), this, SLOT(rejectDialog()));

		/* table that holds every packet sent/received */

		packetTable= new QTableWidget(1, 4, this);

		QStringList headerLabels;
		headerLabels << "Time" << "Header" << "PayloadID"<<"Payload";
		packetTable->setHorizontalHeaderLabels(headerLabels);
		packetTable->setColumnWidth(3,360);
		packetTable->setRowHeight(0,20);

		QGridLayout *packetsGrid = new QGridLayout;
		packetsGrid->addWidget(packetTable,0,1);

		/* Statistics */
		QGridLayout *statsGrid = new QGridLayout;
		statistic_t *communicationStatistics = serialThread->getStatistics();

		QLabel *goodPacketsLabel = new QLabel("Good packets:  " + QString().setNum(communicationStatistics->good));
		QLabel *startByteInconsistencyLabel = new QLabel("Start byte error:  " + QString().number(communicationStatistics->start_byte_inconsistency));
		QLabel *stopByteInconsistencyLabel = new QLabel("Stop byte error:  " + QString().number(communicationStatistics->stop_byte_inconsistency));
		QLabel *escapeByteInconsistencyLabel = new QLabel("Escape byte error:  " + QString().number(communicationStatistics->escape_byte_inconsistency));
		QLabel *badChecksumLabel = new QLabel("Bad checksum:  " + QString().number(communicationStatistics->bad_checksum));
		QLabel *payloadSizeInconsistencyLabel = new QLabel( "Payload size error:  " + QString().number(communicationStatistics->payload_size_inconsistency));

		statsGrid->addWidget(goodPacketsLabel, 0, 0);
		statsGrid->addWidget(startByteInconsistencyLabel, 1, 0);
		statsGrid->addWidget(stopByteInconsistencyLabel,2,0);
		statsGrid->addWidget(escapeByteInconsistencyLabel, 3, 0);
		statsGrid->addWidget(badChecksumLabel, 4, 0);
		statsGrid->addWidget(payloadSizeInconsistencyLabel, 5, 0);

		QTabWidget *tabBar = new QTabWidget();
		QWidget *packetsWidget = new QWidget();
		packetsWidget->setLayout(packetsGrid);
		QWidget *statsTab = new QWidget();
		statsTab->setLayout(statsGrid);

		tabBar->addTab(packetsWidget,"Packets");
		tabBar->addTab(statsTab,"Statistics");

		grid->addWidget(tabBar,1,0,1,2);
		grid->addWidget(accept,3,1);
		dialog->setLayout(grid);
		dialog->show();
	}
}
