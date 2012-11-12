#pragma once


class TiXmlDocument;
class TiXmlHandle;
class TiXmlElement;


class XML
{
public:
	class Element;

	class Document
	{
	public:
		Document();
		Document(const std::string& path);
		~Document();

		void create(const std::string& version, const std::string& encoding, const std::string& standAlone);
		void saveFile(const char* path);
		void saveFile(const std::string& path);
		Element addElement(const std::string& name);

		bool isValid() const;

		Element getElement(const std::string& name);

	private:
		TiXmlDocument* mDocument;
		TiXmlHandle* mDocumentHandle;
	};

	class Element
	{
	public:
		friend class Document;
		bool isValid() const;
		std::string name() const;

		Element first() const;
		
		bool hasNext() const;
		Element next();

		std::string attribute(const char* name) const;

		std::string attribute(const std::string& name) const;

		template <class T>
		void attribute(const char* name, T& value) const;

		template <class T>
		void attribute(const std::string& name, T& value) const;

		void attribute(const char* name, float& value) const;

		void attribute(const std::string& name, float& value) const;

		Element addElement(const std::string& name);

		template <class T>
		void setAttribute(const char* name, const T& value);

		void setAttribute(const char* name, const double& value);

		void setAttribute(const char* name, const float& value);

		template <class T>
		void setAttribute(const std::string& name, const T& value);

	private:
		Element(TiXmlElement* element);

		TiXmlElement* mTiXmlElement;
		bool mValid;
	};
};
