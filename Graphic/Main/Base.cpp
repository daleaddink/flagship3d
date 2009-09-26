#include "Base.h"
#include "DataCenter.h"

namespace Flagship
{
	Key::Key()
	{
		m_szKey = L"";
		m_dwHash = 0;
	}

	Key::Key( wstring& szName )
	{
		DWORD dwHash = 0;
		for ( size_t i = 0 ; i < szName.size(); i++ )
		{
			dwHash = dwHash * 5 + szName.at( i );
		}

		m_szKey = szName.c_str();
		m_dwHash = dwHash;
	}

	Key::~Key()
	{

	}

	void    Key::SetName( wstring& szName )
	{
		DWORD dwHash = 0;
		for ( size_t i = 0 ; i < szName.size(); i++ )
		{
			dwHash = dwHash * 5 + szName.at( i );
		}

		m_szKey = szName.c_str();
		m_dwHash = dwHash;
	}

	bool operator < ( const Key& Left, const Key& Right )
	{
		if ( Left.m_dwHash != Right.m_dwHash )
		{
			return Left.m_dwHash < Right.m_dwHash;
		}

		if ( Left.m_szKey.length() != Right.m_szKey.length() )
		{
			return Left.m_szKey.length() < Right.m_szKey.length();
		}

		return Left.m_szKey < Right.m_szKey;
	}

	bool    Key::operator == ( const Key& Right )
	{
		return m_szKey == Right.m_szKey;
	}

	Key&    Key::operator = ( const Key& Right )
	{
		m_szKey = Right.m_szKey.c_str();
		m_dwHash = Right.m_dwHash;

		return * this;
	}

	Base::Base()
	{

	}

	Base::~Base()
	{

	}

	void    Base::SetName( wstring& szName )
	{
		m_szKey.SetName( szName );
	}

	void    Base::AutoName()
	{
/*		srand( GetTickCount() );

		do 
		{
			int iKey = rand();
			wchar_t szKey[32];
			wsprintf( szKey, L"%d", iKey );
			m_szKey.SetName( wstring( szKey ) );
		} 
		while( m_mKeyMap.find( m_szKey ) != m_mKeyMap.end() ); */
	}

	Key&    Base::GetKey()
	{
		return m_szKey;
	}

	int    Base::GetClassType()
	{
		return m_iClassType;
	}
}