#pragma once


#include <cassert>
#include <string>
#include "3rdparty/tinyxml/tinyxml.h"


class XML
{
public:
	class Element;

	class Document
	{
	public:
		Document()
			: mDocument(0), mDocumentHandle(0)
		{ }

		Document(const std::string& path)
			: mDocument(0), mDocumentHandle(0)
		{
			mDocument = new TiXmlDocument(path.c_str());

			if (mDocument && mDocument->LoadFile())
			{
				mDocumentHandle = new TiXmlHandle(mDocument);
			}
		}

		~Document()
		{
			delete mDocument;
			delete mDocumentHandle;
		}

		void create(const std::string& version, const std::string& encoding, const std::string& standAlone)
		{
			mDocument = new TiXmlDocument();
			mDocument->LinkEndChild(new TiXmlDeclaration(version.c_str(), encoding.c_str(), standAlone.c_str()));  
		}
		
		void saveFile(const char* path)
		{
			mDocument->SaveFile(path);
		}

		void saveFile(const std::string& path)
		{
			saveFile(path.c_str());
		}

		Element addElement(const std::string& name)
		{
			assert(mDocument);
			TiXmlElement* e = new TiXmlElement(name.c_str());
			mDocument->LinkEndChild(e);

			return Element(e);
		}

		bool isValid() const { return mDocumentHandle != 0; }

		Element getElement(const std::string& name)
		{
			assert(mDocumentHandle != 0);
			TiXmlElement* element = mDocumentHandle->FirstChildElement(name.c_str()).Element();
			return Element(element);
		}

	private:
		TiXmlDocument* mDocument;
		TiXmlHandle* mDocumentHandle;
	};

	class Element
	{
	public:
		friend class Document;
		bool isValid() const { return mValid; }
		std::string name() const { return std::string(mTiXmlElement->Value()); }

		Element first() const { return Element(mTiXmlElement->FirstChildElement()); }
		
		bool hasNext() const { return (mTiXmlElement->NextSiblingElement() != 0); }
		Element next() { return Element(mTiXmlElement->NextSiblingElement()); }

		std::string attribute(const char* name) const
		{
			const char* str = mTiXmlElement->Attribute(name);
			if (str != 0)
			{
				return std::string(str);
			}

			return std::string();
		}

		std::string attribute(const std::string& name) const
		{
			return attribute(name.c_str());
		}

		template <class T>
		void attribute(const char* name, T& value) const
		{
			mTiXmlElement->Attribute(name, &value);
		}

		template <class T>
		void attribute(const std::string& name, T& value) const
		{
			attribute(name.c_str(), value);
		}

		void attribute(const char* name, float& value) const
		{
			double Value = 0.0;
			mTiXmlElement->Attribute(name, &Value);
			value = static_cast<float>(Value);
		}

		void attribute(const std::string& name, float& value) const
		{
			attribute(name.c_str(), value);
		}

		Element addElement(const std::string& name)
		{
			assert(mTiXmlElement);
			TiXmlElement* e = new TiXmlElement(name.c_str());
			mTiXmlElement->LinkEndChild(e);

			return Element(e);
		}

		template <class T>
		void setAttribute(const char* name, const T& value)
		{
			mTiXmlElement->SetAttribute(name, value);
		}

		void setAttribute(const char* name, const double& value)
		{
			mTiXmlElement->SetDoubleAttribute(name, value);
		}

		void setAttribute(const char* name, const float& value)
		{
			mTiXmlElement->SetDoubleAttribute(name, static_cast<double>(value));
		}

		template <class T>
		void setAttribute(const std::string& name, const T& value)
		{
			setAttribute(name.c_str(), value);
		}

	private:
		Element(TiXmlElement* element) : mTiXmlElement(element), mValid(element != 0) {}

		TiXmlElement* mTiXmlElement;
		bool mValid;
	};
};
