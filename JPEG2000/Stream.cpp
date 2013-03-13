
#include "Stream.h"
#include <fcntl.h>
using namespace JPEG2000;

/* ���ַ����м����ļ�����Ȩ�� */
int Stream::CalMode(const char *s)
{
	int openmode = 0;

	while (*s != '\0') {
		switch (*s) {
		case 'r':
			openmode |= STREAM_READ;
			break;
		case 'w':
			openmode |= STREAM_WRITE | STREAM_CREATE;
			break;
		case 'b':
			openmode |= STREAM_BINARY;
			break;
		case 'a':
			openmode |= STREAM_APPEND;
			break;
		case 'e':
			openmode|=STREAM_ATE;
			break;
		case '+':
			openmode |= STREAM_READ | STREAM_WRITE;
			break;
		default:
			break;
		}
		++s;
	}

	int openflags=0;
	if ((openmode & STREAM_READ) &&
		(openmode & STREAM_WRITE)) {
			openflags = std::ios::in|std::ios::out;
	} else if (openmode & STREAM_READ) {
		openflags =std::ios::in;
	} else if (openmode & STREAM_WRITE) {
		openflags =std::ios::out;
	} else {
		openflags = 0;
	}

	if (openmode & STREAM_APPEND) {
		openflags |= std::ios::app;
	}

	if(openmode&STREAM_ATE)
	{
		openflags|=std::ios::ate;
	}

	if (openmode & STREAM_BINARY) {
		openflags |= std::ios::binary;
	}
	if (openmode & STREAM_CREATE) {
		openflags |= std::ios::trunc;//create 
	}

	return openflags;
}

Stream::Stream(wchar_t* fileName,const char *mode)
{
	this->m_openMode=0;
	this->m_bufMode=0;
	this->m_bufSize=0;
	this->m_count=0;
	this->m_rwCount=0;
	this->m_rwLimit=0;
	this->ptr=0;
	this->p_bufBase=0;
	this->p_bufStart=0;
	this->m_flags=this->CalMode(mode);
	this->p_fileName=fileName;
	this->stream=NULL;
	this->m_size=0;
}

Stream::~Stream()
{

}

int Stream::open()
{
	this->stream=new std::fstream;
	this->stream->open(this->p_fileName,this->m_flags);
	if(!this->stream->good())
	{
		Logger::error("open file failed");
		return 0;
	}

	std::streamoff pos=this->stream->tellg();
	
	this->m_size=this->stream->tellg();
	this->stream->seekg(0,std::ios::beg);
	return 1;
}

int Stream::read(char *&buffer)
{
	if(this->stream==NULL)
	{
		return -1;
	}

	buffer=new char[this->m_size];
	this->stream->read(buffer,this->m_size);

	return 1;
}

int Stream::write(const char *data,long size,bool app)
{
	if(this->stream!=NULL&&!this->canWrite())
	{
		return -1;
	}
	if(app)
	{
		this->stream->seekp(0,std::ios::end);
	}else{
		this->stream->seekp(0,std::ios::beg);
	}

	this->stream->write(data,size);

	return 1;
}

void Stream::setStream(std::fstream *in)
{
	this->stream=in;
}

std::fstream* Stream::getStream()
{
	return this->stream;
}

bool Stream::canWrite()
{
	if(this->m_flags&std::ios::out)
	{
		return true;
	}
	return false;
}