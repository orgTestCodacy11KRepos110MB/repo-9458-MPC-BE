/*
 * (C) 2003-2006 Gabest
 * (C) 2006-2022 see Authors.txt
 *
 * This file is part of MPC-BE.
 *
 * MPC-BE is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * MPC-BE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <afx.h>

class CTextFile : protected CStdioFile
{
public:
	enum enc {
		ASCII,
		UTF8,
		LE16,
		BE16,
		ANSI
	};

private:
	enc m_encoding, m_defaultencoding;
	int m_offset;
	ULONGLONG m_posInFile;
	std::unique_ptr<char[]> m_buffer;
	std::unique_ptr<WCHAR[]> m_wbuffer;
	LONGLONG m_posInBuffer, m_nInBuffer;

public:
	CTextFile(enc encoding = ASCII, enc defaultencoding = ASCII);

	virtual bool Open(LPCWSTR lpszFileName);
	virtual bool Save(LPCWSTR lpszFileName, enc e /*= ASCII*/);
	virtual void Close() { return __super::Close(); };

	void SetEncoding(enc e);
	enc GetEncoding();
	bool IsUnicode();

	// CFile

	CStringW GetFilePath() const;

	// CStdioFile

	ULONGLONG GetPosition() const;
	ULONGLONG GetLength() const;
	ULONGLONG Seek(LONGLONG lOff, UINT nFrom);

	void WriteString(LPCSTR lpsz/*CStringA str*/);
	void WriteString(LPCWSTR lpsz/*CStringW str*/);
	BOOL ReadString(CStringA& str);
	BOOL ReadString(CStringW& str);

protected:
	virtual bool ReopenAsText();
	bool FillBuffer();
	ULONGLONG GetPositionFastBuffered() const;
};

class CWebTextFile : public CTextFile
{
	LONGLONG m_llMaxSize;
	CStringW m_tempfn;

	CString m_url_redirect_str;

public:
	CWebTextFile(enc encoding = ASCII, enc defaultencoding = ASCII, LONGLONG llMaxSize = 1024 * 1024);
	~CWebTextFile();

	bool Open(LPCWSTR lpszFileName);
	bool Save(LPCWSTR lpszFileName, enc e /*= ASCII*/);
	void Close();

	const CString& GetRedirectURL() const;
};

extern CStringW AToT(CStringA str);
extern CStringA TToA(CStringW str);
