
// qt headers
#include <QtCore/QStringList>

// local headers
#include "message.h"
#include "header/header.h"
#include "question/question.h"
#include "resourcerecord/resourcerecord.h"

Message::Message():
	_header(0),
	_question(0)
{
}

bool Message::fromByteArray( QByteArray& byteArray )
{
	cleanUp();
	
	int indexInBuffer = 0;

	// filling the header
	_header = new Header();
	bool result = _header->fromByteArray( byteArray, indexInBuffer );
	if( false == result )
		return false;

	// filling the question part
	if( _header->getQDcount() != 0 )
	{
		_question = new Question();
		result = _question->fromByteArray( byteArray, indexInBuffer );
		if( false == result )
			return false;
	}
	
	// filling the answer part
	if( _header->getANcount() != 0 )
	{
		int answerCount = 0;
		for( ; answerCount < _header->getANcount(); ++answerCount )
		{
			ResourceRecord* resourceRecord = new ResourceRecord();
			result = resourceRecord->fromByteArray( byteArray, indexInBuffer );
			if( true == result )
				_answers.append( resourceRecord );
		}
	}

	// filling the authoritative nameservers part
	if( _header->getNScount() != 0 )
	{
		int authorityCount = 0;
		for( ; authorityCount < _header->getNScount(); ++authorityCount )
		{
			ResourceRecord* resourceRecord = new ResourceRecord();
			result = resourceRecord->fromByteArray( byteArray, indexInBuffer );
			if( true == result )
				_authorities.append( resourceRecord );
		}
	}

	// filling the additional part
	if( _header->getARcount() != 0 )
	{
		int additionalCount = 0;
		for( ; additionalCount < _header->getARcount(); ++additionalCount )
		{
			ResourceRecord* resourceRecord = new ResourceRecord();
			result = resourceRecord->fromByteArray( byteArray, indexInBuffer );
			if( true == result )
				_additionals.append( resourceRecord );
		}
	}
	
	return true;
}

QByteArray Message::toByteArray()
{
	QByteArray byteArray;

	byteArray  = _header->toByteArray();
	byteArray += _question->toByteArray();
	
	for( int index = 0; index < _answers.size(); index++ )
		byteArray += _answers.at(index)->toByteArray();

	for( int index = 0; index < _authorities.size(); index++ )
		byteArray += _authorities.at(index)->toByteArray();
		
	for( int index = 0; index < _additionals.size(); index++ )
		byteArray += _additionals.at(index)->toByteArray();

	return byteArray;
}

void Message::buildQueryMessagetForARecord( QByteArray domainName )
{
}

void Message::buildQueryMessagetForMXRecord( QByteArray domainName )
{
	cleanUp();

	init();

	// setting the header
	_header->setID( Header::ID::generateID() );
	_header->setQR( Header::QR::Query );
	_header->setOpcode( Header::OPCODE::StandardQuery );
	_header->setAA( Header::AA::NonAuthoritativeAnswer );
	_header->setTC( Header::TC::NoTruncation );
	_header->setRD( Header::RD::RecursionDesired );
	_header->setRA( Header::RA::NoRecursionAvailable );
	_header->setRcode( Header::RCODE::NoError );
	_header->setQDcount( 1 );
	_header->setANcount( 0 );
	_header->setNScount( 0 );
	_header->setARcount( 0 );	
	
	// setting the question
	_question->setQname(  domainName );
	_question->setQtype(  Question::QTYPE::MX );
	_question->setQclass( Question::QCLASS::CS );	
}

void Message::getServers( QStringList& serverList )
{	
	foreach( ResourceRecord* resourceRecord, _additionals )
	{
		QByteArray byteArray = resourceRecord->getName();
		serverList.append( byteArray );
	}
}

void Message::init()
{
	_header = new Header();
	_question = new Question();
}

void Message::cleanUp()
{
	if( 0 != _header )
		delete _header;
	
	if( 0 != _question )
		delete _question;

	for( int index = 0; index < _answers.size(); index++ )
		delete _answers.at(index);
	_answers.clear();

	for( int index = 0; index < _authorities.size(); index++ )
		delete _authorities.at(index);
	_authorities.clear();
		
	for( int index = 0; index < _additionals.size(); index++ )
		delete _additionals.at(index);
	_additionals.clear();

	_header = 0;
	_question = 0;
}

Message::~Message()
{
	cleanUp();
}
