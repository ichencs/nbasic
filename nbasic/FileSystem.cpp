#include "stdafx.h"
#include "FileSystem.h"
#include "NLocale.h"
#include "Collections/OperationForEach.h"
#include "Stream/FileStream.h"
#include "Stream/Accessor.h"
#include "Exception.h"
#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

		// ReadDirectoryChangesW

/***********************************************************************
FilePath
***********************************************************************/


		void NFilePath::Initialize()
		{
			{
				NArray<wchar_t> buffer(fullPath.Length() + 1);
				wcscpy_s(&buffer[0], fullPath.Length() + 1, fullPath.Buffer());
				for (nint i = 0; i < buffer.Count(); i++)
				{
					if (buffer[i] == L'\\' || buffer[i] == L'/')
					{
						buffer[i] = Delimiter;
					}
				}
				fullPath = &buffer[0];
			}

			if (fullPath != L"")
			{
				if (fullPath.Length() < 2 || fullPath[1] != L':')
				{
					wchar_t buffer[MAX_PATH + 1] = { 0 };
					DWORD result = GetCurrentDirectory(sizeof(buffer) / sizeof(*buffer), buffer);
					if (result > MAX_PATH + 1 || result == 0)
					{
						throw ArgumentException(L"Failed to call GetCurrentDirectory.", L"vl::filesystem::FilePath::Initialize", L"");
					}
					fullPath = WString(buffer) + L"\\" + fullPath;
				}
				{
					wchar_t buffer[MAX_PATH + 1] = { 0 };
					if (fullPath.Length() == 2 && fullPath[1] == L':')
					{
						fullPath += L"\\";
					}
					DWORD result = GetFullPathName(fullPath.Buffer(), sizeof(buffer) / sizeof(*buffer), buffer, NULL);
					if (result > MAX_PATH + 1 || result == 0)
					{
						throw ArgumentException(L"The path is illegal.", L"vl::filesystem::FilePath::FilePath", L"_filePath");
					}
					fullPath = buffer;
				}
			}

			if (fullPath != L"/" && fullPath.Length() > 0 && fullPath[fullPath.Length() - 1] == Delimiter)
			{
				fullPath = fullPath.Left(fullPath.Length() - 1);
			}
		}

		NFilePath::NFilePath()
		{
		}

		NFilePath::NFilePath(const WString& _filePath)
			:fullPath(_filePath)
		{
			Initialize();
		}

		NFilePath::NFilePath(const wchar_t* _filePath)
			:fullPath(_filePath)
		{
			Initialize();
		}

		NFilePath::NFilePath(const NFilePath& _filePath)
			:fullPath(_filePath.fullPath)
		{
			Initialize();
		}

		NFilePath::~NFilePath()
		{
		}

		nint NFilePath::Compare(const NFilePath& a, const NFilePath& b)
		{
			return WString::Compare(a.fullPath, b.fullPath);
		}

		NFilePath NFilePath::operator/(const WString& relativePath)const
		{
			if (IsRoot())
			{
				return relativePath;
			}
			else
			{
				return fullPath + L"/" + relativePath;
			}
		}

		bool NFilePath::IsFile()const
		{
			WIN32_FILE_ATTRIBUTE_DATA info;
			BOOL result = GetFileAttributesEx(fullPath.Buffer(), GetFileExInfoStandard, &info);
			if (!result) return false;
			return (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
		}

		bool NFilePath::IsFolder()const
		{
			WIN32_FILE_ATTRIBUTE_DATA info;
			BOOL result = GetFileAttributesEx(fullPath.Buffer(), GetFileExInfoStandard, &info);
			if (!result) return false;
			return (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
		}

		bool NFilePath::IsRoot()const
		{
			return fullPath == L"";
		}

		WString NFilePath::GetName()const
		{
			WString delimiter = Delimiter;
			NPair<nint, nint> index = INVLOC.FindLast(fullPath, delimiter, NLocale::None);
			if (index.key == -1) return fullPath;
			return fullPath.Right(fullPath.Length() - index.key - 1);
		}

		NFilePath NFilePath::GetFolder()const
		{
			WString delimiter = Delimiter;
			NPair<nint, nint> index = INVLOC.FindLast(fullPath, delimiter, NLocale::None);
			if (index.key == -1) return NFilePath();
			return fullPath.Left(index.key);
		}

		WString NFilePath::GetFullPath()const
		{
			return fullPath;
		}

		WString NFilePath::GetRelativePathFor(const NFilePath& _filePath)
		{
			if (fullPath.Length()==0 || _filePath.fullPath.Length()==0 || fullPath[0] != _filePath.fullPath[0])
			{
				return _filePath.fullPath;
			}
			wchar_t buffer[MAX_PATH + 1] = { 0 };
			PathRelativePathTo(
				buffer,
				fullPath.Buffer(),
				(IsFolder() ? FILE_ATTRIBUTE_DIRECTORY : 0),
				_filePath.fullPath.Buffer(),
				(_filePath.IsFolder() ? FILE_ATTRIBUTE_DIRECTORY : 0)
				);
			return buffer;
		}

		void NFilePath::GetPathComponents(WString path,  NList<WString>& components)
		{
			WString pathRemaining = path;
			WString delimiter = Delimiter;

			components.Clear();

			while(true)
			{
				NPair<nint, nint> index = INVLOC.FindFirst(pathRemaining, delimiter, NLocale::None);
				if (index.key == -1)
					break;

				if(index.key != 0)
					components.Add(pathRemaining.Left(index.key));
				else
				{
					if(pathRemaining.Length() >= 2 && pathRemaining[1] == Delimiter)
					{
						// Windows UNC Path starting with "\\"
						// components[0] will be L"\\"
						components.Add(L"\\");
						index.value++;
					}
				}

				pathRemaining = pathRemaining.Right(pathRemaining.Length() - (index.key + index.value));
			}

			if(pathRemaining.Length() != 0)
			{
				components.Add(pathRemaining);
			}
		}

		WString NFilePath::ComponentsToPath(const  NList<WString>& components)
		{
			WString result;
			WString delimiter = Delimiter;

			int i = 0;

			// For Windows, if first component is "\\" then it is an UNC path
			if(components.Count() > 0 && components[0] == L"\\")
			{
				result += delimiter;
				i++;
			}

			for(; i < components.Count(); i++)
			{
				result += components[i];
				if(i + 1 < components.Count())
					result += delimiter;
			}

			return result;
		}

/***********************************************************************
File
***********************************************************************/

		NFile::NFile()
		{
		}
		
		NFile::NFile(const NFilePath& _filePath)
			:filePath(_filePath)
		{
		}

		NFile::~NFile()
		{
		}

		const NFilePath& NFile::GetFilePath()const
		{
			return filePath;
		}

		bool NFile::ReadAllTextWithEncodingTesting(WString& text, BomEncoder::Encoding& encoding, bool& containsBom)
		{
			NArray<unsigned char> buffer;
			{
				FileStream fileStream(filePath.GetFullPath(), FileStream::ReadOnly);
				if (!fileStream.IsAvailable()) return false;
				if (fileStream.Size() == 0)
				{
					text = L"";
					encoding = BomEncoder::Mbcs;
					containsBom = false;
					return true;
				}

				buffer.Resize((nint)fileStream.Size());
				nint count = fileStream.Read(&buffer[0], buffer.Count());
				CHECK_ERROR(count == buffer.Count(), L"vl::filesystem::File::ReadAllTextWithEncodingTesting(WString&, BomEncoder::Encoding&, bool&)#Failed to read the whole file.");
			}
			TestEncoding(&buffer[0], buffer.Count(), encoding, containsBom);

			MemoryWrapperStream memoryStream(&buffer[0], buffer.Count());
			if (containsBom)
			{
				BomDecoder decoder;
				DecoderStream decoderStream(memoryStream, decoder);
				StreamReader reader(decoderStream);
				text = reader.ReadToEnd();
			}
			else
			{
				switch (encoding)
				{
				case BomEncoder::Utf8:
					{
						Utf8Decoder decoder;
						DecoderStream decoderStream(memoryStream, decoder);
						StreamReader reader(decoderStream);
						text = reader.ReadToEnd();
					}
					break;
				case BomEncoder::Utf16:
					{
						Utf16Decoder decoder;
						DecoderStream decoderStream(memoryStream, decoder);
						StreamReader reader(decoderStream);
						text = reader.ReadToEnd();
					}
					break;
				case BomEncoder::Utf16BE:
					{
						Utf16BEDecoder decoder;
						DecoderStream decoderStream(memoryStream, decoder);
						StreamReader reader(decoderStream);
						text = reader.ReadToEnd();
					}
					break;
				default:
					{
						MbcsDecoder decoder;
						DecoderStream decoderStream(memoryStream, decoder);
						StreamReader reader(decoderStream);
						text = reader.ReadToEnd();
					}
				}
			}
			return true;
		}

		WString NFile::ReadAllTextByBom()const
		{
			WString text;
			ReadAllTextByBom(text);
			return text;
		}

		bool NFile::ReadAllTextByBom(WString& text)const
		{
			FileStream fileStream(filePath.GetFullPath(), FileStream::ReadOnly);
			if (!fileStream.IsAvailable()) return false;
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);
			text = reader.ReadToEnd();
			return true;
		}

		bool NFile::ReadAllLinesByBom( NList<WString>& lines)const
		{
			FileStream fileStream(filePath.GetFullPath(), FileStream::ReadOnly);
			if (!fileStream.IsAvailable()) return false;
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);
			while (!reader.IsEnd())
			{
				lines.Add(reader.ReadLine());
			}
			return true;
		}

		bool NFile::WriteAllText(const WString& text, bool bom, BomEncoder::Encoding encoding)
		{
			FileStream fileStream(filePath.GetFullPath(), FileStream::WriteOnly);
			if (!fileStream.IsAvailable()) return false;
			
			IEncoder* encoder = 0;
			if (bom)
			{
				encoder = new BomEncoder(encoding);
			}
			else switch (encoding)
			{
			case BomEncoder::Utf8:
				encoder = new Utf8Encoder;
				break;
			case BomEncoder::Utf16:
				encoder = new Utf16Encoder;
				break;
			case BomEncoder::Utf16BE:
				encoder = new Utf16BEEncoder;
				break;
			default:
				encoder = new MbcsEncoder;
				break;
			}

			{
				EncoderStream encoderStream(fileStream, *encoder);
				StreamWriter writer(encoderStream);
				writer.WriteString(text);
			}
			delete encoder;
			return true;
		}

		bool NFile::WriteAllLines( NList<WString>& lines, bool bom, BomEncoder::Encoding encoding)
		{
			FileStream fileStream(filePath.GetFullPath(), FileStream::WriteOnly);
			if (!fileStream.IsAvailable()) return false;
			
			IEncoder* encoder = NULL;
			if (bom)
			{
				encoder = new BomEncoder(encoding);
			}
			else switch (encoding)
			{
			case BomEncoder::Utf8:
				encoder = new Utf8Encoder;
				break;
			case BomEncoder::Utf16:
				encoder = new Utf16Encoder;
				break;
			case BomEncoder::Utf16BE:
				encoder = new Utf16BEEncoder;
				break;
			default:
				encoder = new MbcsEncoder;
				break;
			}

			{
				EncoderStream encoderStream(fileStream, *encoder);
				StreamWriter writer(encoderStream);
				FOREACH(WString, line, lines)
				{
					writer.WriteLine(line);
				}
			}
			delete encoder;
			return true;
		}

		bool NFile::Exists()const
		{
			return filePath.IsFile();
		}

		bool NFile::Delete()const
		{
			return DeleteFile(filePath.GetFullPath().Buffer()) != 0;
		}

		bool NFile::Rename(const WString& newName)const
		{
			WString oldFileName = filePath.GetFullPath();
			WString newFileName = (filePath.GetFolder() / newName).GetFullPath();
			return MoveFile(oldFileName.Buffer(), newFileName.Buffer()) != 0;
		}

/***********************************************************************
Folder
***********************************************************************/

		NFolder::NFolder()
		{
		}
		
		NFolder::NFolder(const NFilePath& _filePath)
			:filePath(_filePath)
		{
		}

		NFolder::~NFolder()
		{
		}

		const NFilePath& NFolder::GetFilePath()const
		{
			return filePath;
		}

		bool NFolder::GetFolders(NList<NFolder>& folders)const
		{
			if (filePath.IsRoot())
			{
				DWORD bufferSize = GetLogicalDriveStrings(0, NULL);
				if (bufferSize > 0)
				{
					NArray<wchar_t> buffer(bufferSize);
					if (GetLogicalDriveStrings((DWORD)buffer.Count(), &buffer[0]) > 0)
					{
						wchar_t* begin = &buffer[0];
						wchar_t* end = begin + buffer.Count();
						while (begin < end && *begin)
						{
							WString driveString = begin;
							begin += driveString.Length() + 1;
							folders.Add(NFolder(NFilePath(driveString)));
						}
						return true;
					}
				}
				return false;
			}
			else
			{
				if (!Exists()) return false;
				WIN32_FIND_DATA findData;
				HANDLE findHandle = INVALID_HANDLE_VALUE;

				while (true)
				{
					if (findHandle == INVALID_HANDLE_VALUE)
					{
						WString searchPath = (filePath / L"*").GetFullPath();
						findHandle = FindFirstFile(searchPath.Buffer(), &findData);
						if (findHandle == INVALID_HANDLE_VALUE)
						{
							break;
						}
					}
					else
					{
						BOOL result = FindNextFile(findHandle, &findData);
						if (result == 0)
						{
							FindClose(findHandle);
							break;
						}
					}

					if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						if (wcscmp(findData.cFileName, L".") != 0 && wcscmp(findData.cFileName, L"..") != 0)
						{
							folders.Add(NFolder(filePath / findData.cFileName));
						}
					}
				}
				return true;
			}
		}

		bool NFolder::GetFiles( NList<NFile>& files)const
		{
			if (filePath.IsRoot())
			{
				return true;
			}
			if (!Exists()) return false;
			WIN32_FIND_DATA findData;
			HANDLE findHandle = INVALID_HANDLE_VALUE;

			while (true)
			{
				if (findHandle == INVALID_HANDLE_VALUE)
				{
					WString searchPath = (filePath / L"*").GetFullPath();
					findHandle = FindFirstFile(searchPath.Buffer(), &findData);
					if (findHandle == INVALID_HANDLE_VALUE)
					{
						break;
					}
				}
				else
				{
					BOOL result = FindNextFile(findHandle, &findData);
					if (result == 0)
					{
						FindClose(findHandle);
						break;
					}
				}

				if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					files.Add(NFile(filePath / findData.cFileName));
				}
			}
			return true;
		}

		bool NFolder::Exists()const
		{
			return filePath.IsFolder();
		}

		bool NFolder::Create(bool recursively)const
		{
			if (recursively)
			{
				NFilePath folder = filePath.GetFolder();
				if (folder.IsFile()) return false;
				if (folder.IsFolder()) return Create(false);
				return NFolder(folder).Create(true) && Create(false);
			}
			else
			{
				return CreateDirectory(filePath.GetFullPath().Buffer(), NULL) != 0;
			}
		}

		bool NFolder::Delete(bool recursively)const
		{
			if (!Exists()) return false;
			
			if (recursively)
			{
				NList<NFolder> folders;
				GetFolders(folders);
				FOREACH(NFolder, folder, folders)
				{
					if (!folder.Delete(true)) return false;
				}
				
				NList<NFile> files;
				GetFiles(files);
				FOREACH(NFile, file, files)
				{
					if (!file.Delete()) return false;
				}

				return Delete(false);
			}
			return RemoveDirectory(filePath.GetFullPath().Buffer()) != 0;
		}

		bool NFolder::Rename(const WString& newName)const
		{
			WString oldFileName = filePath.GetFullPath();
			WString newFileName = (filePath.GetFolder() / newName).GetFullPath();
			return MoveFile(oldFileName.Buffer(), newFileName.Buffer()) != 0;
		}
