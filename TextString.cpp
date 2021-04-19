#include <iostream>
#include <stdio.h>



class TextString
{
protected:
	char *str;

public:
	TextString();
	TextString(const TextString &incoming); //copy constructor
	TextString &operator=(const TextString &incoming); //copy operator
	~TextString();
	void clear(void);
	int size(void) const;
	void push_back(char c);
	void pop_back(void);
	const char *data(void) const;
	const char *c_str(void) const;

	const char *fgets(FILE *fp);  // <-> std::getline

	void copy_from(const TextString &incoming);
	void set(const char incoming[]);
};

TextString::TextString()
{
	str=nullptr;
}
TextString::~TextString()
{
	clear();
}
void TextString::clear(void)
{
	delete [] str;
	str=nullptr;
}
int TextString::size(void) const
{
	if(nullptr==str)
	{
		return 0;
	}
	int i;
	for(i=0; 0!=str[i]; ++i)
	{
	}
	return i;
}
void TextString::push_back(char c)
{
	auto s=size();
	// Allocate
	char *newStr=new char [s+2];
	// Copy
	for(int i=0; i<s; ++i)
	{
		newStr[i]=str[i];
	}
	// Append
	newStr[s]=c;
	newStr[s+1]=0;  // Internally C-string, therefore needs terminator.

	// Clear
	clear();

	// Repoint
	str=newStr;
}
void TextString::pop_back(void)
{
	auto s=size();
	if(0<s)
	{
		// str[s]==0
		str[s-1]=0;

		// Allocate
		char *newStr=new char [s];

		// Copy
		for(int i=0; i<s; ++i)
		{
			newStr[i]=str[i];
		}

		// Clear
		clear();

		// Repoint
		str=newStr;
	}
}
const char *TextString::data(void) const
{
	return (nullptr==str ? "" : str);
}
const char *TextString::c_str(void) const
{
	return (nullptr==str ? "" : str);
}

const char *TextString::fgets(FILE *fp)
{
	const int bufLen=16;
	char buf[16];
	bool filled=false;

	this->clear();

	while(nullptr!=::fgets(buf,15,fp))
	{
		filled=true;
		for(int i=0; 0!=buf[i]; ++i)
		{
			if(buf[i]=='\n' || buf[i]=='\r')
			{
				goto EXIT;
			}
			this->push_back(buf[i]);
		}
	}

EXIT:
	if(true==filled)
	{
		return this->data();
	}
	return nullptr;
}



TextString::TextString(const TextString &incoming)
{
std::cout << __FUNCTION__ << std::endl;
	str=nullptr;
	copy_from(incoming);
}
TextString &TextString::operator=(const TextString &incoming)
{
std::cout << __FUNCTION__ << std::endl;
	copy_from(incoming);
	return *this;
}
void TextString::copy_from(const TextString &incoming)
{
	set(incoming.c_str());
}
void TextString::set(const char incoming[])
{
	if(incoming!=str)
	{
		clear();
		if(nullptr!=incoming)
		{
			for(int i=0; 0!=incoming[i]; ++i)
			{
				push_back(incoming[i]);
			}
		}
	}
}



void CopyTest(void)
{
	TextString s1,s2;

	s1.push_back('C');
	s1.push_back('M');
	s1.push_back('U');

	s2=s1;

	std::cout << "s2=" << s2.data() << std::endl;

	TextString s3(s2);
	std::cout << "s3=" << s3.data() << std::endl;
}



int main(void)
{
/*
	TextString s;  // <-> std::string
	s.push_back('C');
	s.push_back('M');
	s.push_back('U');
	std::cout << s.data() << std::endl;

	s.pop_back();
	std::cout << s.data() << std::endl;
	s.pop_back();
	std::cout << s.data() << std::endl;
	s.pop_back();
	std::cout << s.data() << std::endl;

	s.pop_back();
	std::cout << s.data() << std::endl;
*/

	FILE *fp=fopen("TextString.cpp","r");
	TextString s;
	while(nullptr!=s.fgets(fp))  // while(nullptr!=fgets(buf,255,fp))
	{
		std::cout << "> " << s.data() << std::endl;
	}

	CopyTest();

	std::cout << "Peacefully ended." << std::endl;

	return 0;
}
