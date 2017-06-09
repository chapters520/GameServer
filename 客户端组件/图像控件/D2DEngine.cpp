#include "StdAfx.h"
#include ".\d2dengine.h"

CD2DEngine*				CD2DEngine::m_pObject = NULL;
bool					CD2DEngine::m_bCreate = false;
CD2DEngine::CD2DEngine(void)
{
	m_hInst = NULL;
	m_pD2DInterface = NULL;
	m_pCurrentWnd = NULL;

	m_hInst = LoadLibrary(TEXT("D2DEngine.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(); 
		CREATE ClientControl = (CREATE)GetProcAddress(m_hInst,"CreateD2DEngine"); 
		if ( ClientControl )
		{
			m_pD2DInterface = static_cast<ID2DInterface*>(ClientControl());
		}
	}
}

CD2DEngine::~CD2DEngine(void)
{
	if ( m_pD2DInterface )
	{
		m_pD2DInterface->Release();
		m_pD2DInterface = NULL;
	}
	if( m_hInst )
	{
		FreeLibrary(m_hInst);
		m_hInst = NULL;
	}
	m_pCurrentWnd = NULL;
}
