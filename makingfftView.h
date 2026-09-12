
// makingfftView.h: CmakingfftView 클래스의 인터페이스
//

#pragma once
#include <atlimage.h>
#include "FFTEngine.h"

class CmakingfftView : public CView
{
protected: // serialization에서만 만들어집니다.
	CmakingfftView() noexcept;
	DECLARE_DYNCREATE(CmakingfftView)
	CImage m_image;
	CImage m_fftImage; 

// 특성입니다.
public:
	CmakingfftDoc* GetDocument() const;
	afx_msg void OnFileOpen();
	afx_msg void OnProcessFft();
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CmakingfftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // makingfftView.cpp의 디버그 버전
inline CmakingfftDoc* CmakingfftView::GetDocument() const
   { return reinterpret_cast<CmakingfftDoc*>(m_pDocument); }
#endif

