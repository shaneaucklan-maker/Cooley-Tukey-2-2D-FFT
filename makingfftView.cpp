
// makingfftView.cpp: CmakingfftView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "makingfft.h"
#endif

#include "makingfftDoc.h"
#include "makingfftView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmakingfftView

IMPLEMENT_DYNCREATE(CmakingfftView, CView)

BEGIN_MESSAGE_MAP(CmakingfftView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CmakingfftView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()

	ON_COMMAND(ID_FILE_OPEN, &CmakingfftView::OnFileOpen)
	ON_COMMAND(ID_PROCESS_FFT, &CmakingfftView::OnProcessFft)
	

END_MESSAGE_MAP()

// CmakingfftView 생성/소멸

CmakingfftView::CmakingfftView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CmakingfftView::~CmakingfftView()
{
}

BOOL CmakingfftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CmakingfftView 그리기

void CmakingfftView::OnDraw(CDC*pDC)
{
	CmakingfftDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	// 로드된 BMP가 있을 경우 화면에 출력
	int startX = 10;

	// 원본 이미지 출력
	if (!m_image.IsNull()) {
		m_image.Draw(pDC->GetSafeHdc(), startX, 10);
		pDC->TextOutW(startX, 15 + m_image.GetHeight(), _T("[Original Image]"));
		startX += m_image.GetWidth() + 30; // 원본 이미지 너비만큼 우측 이동
	}

	// FFT 변환 결과 이미지 출력 (원본 우측)
	if (!m_fftImage.IsNull()) {
		m_fftImage.Draw(pDC->GetSafeHdc(), startX, 10);
		pDC->TextOutW(startX, 15 + m_fftImage.GetHeight(), _T("[2D FFT Spectrum]"));
	}
}


// CmakingfftView 인쇄


void CmakingfftView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CmakingfftView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CmakingfftView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CmakingfftView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CmakingfftView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CmakingfftView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CmakingfftView 진단

#ifdef _DEBUG
void CmakingfftView::AssertValid() const
{
	CView::AssertValid();
}

void CmakingfftView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CmakingfftDoc* CmakingfftView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmakingfftDoc)));
	return (CmakingfftDoc*)m_pDocument;
}
#endif //_DEBUG


// CmakingfftView 메시지 처리기
void CmakingfftView::OnFileOpen()
{
	// CFileDialog: MFC 표준 파일 선택 Dialog 창을 띄웁니다.
	CFileDialog dlg(TRUE, _T("bmp"), NULL, OFN_FILEMUSTEXIST,
		_T("BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"), this);

	// Dialog 창에서 [확인] 버튼을 눌렀을 경우
	if (dlg.DoModal() == IDOK)
	{
		// 기존 이미지가 채워져 있다면 메모리 해제
		if (!m_image.IsNull())
		{
			m_image.Destroy();
		}

		// Dialog에서 선택한 경로로 BMP 파일 로드
		CString filePath = dlg.GetPathName();
		HRESULT hr = m_image.Load(filePath);

		if (SUCCEEDED(hr))
		{
			Invalidate(); // 화면 전체 재그리기 요청 -> OnDraw() 자동 실행
		}
		else
		{
			AfxMessageBox(_T("이미지 로드 실패"));
		}
	}
}

// 1. FFT 연산 및 결과 CImage 생성
void CmakingfftView::OnProcessFft()
{
	if (m_image.IsNull()) {
		AfxMessageBox(_T("먼저 이미지를 로드하세요."));
		return;
	}


	int origW = m_image.GetWidth();
	int origH = m_image.GetHeight();

	// FFT는 가로/세로 크기가 2의 거듭제곱(Power of 2)이어야 하므로 크기 재조정
	int fftW = CFFTEngine::GetNextPowerOfTwo(origW);
	int fftH = CFFTEngine::GetNextPowerOfTwo(origH);

	
	// Grayscale 변환 및 복소수 2차원 배열 준비
	std::vector<std::vector<Complex>> fftData(fftH, std::vector<Complex>(fftW, 0.0));

	for (int y = 0; y < origH; ++y) {
		for (int x = 0; x < origW; ++x) {
			COLORREF color = m_image.GetPixel(x, y);
			// GrayScale 명도 공식 (0.299R + 0.587G + 0.114B)
			double gray = 0.299 * GetRValue(color) + 0.587 * GetGValue(color) + 0.114 * GetBValue(color);
			fftData[y][x] = Complex(gray, 0.0);
		}
	}
	
	// 2D FFT 실행
	CFFTEngine::FFT2D(fftData, fftW, fftH);
	



	// Magnitude(스펙트럼 크기) 계산 및 로그 압축 [ log(1 + |F|) ]
	std::vector<std::vector<double>> magnitude(fftH, std::vector<double>(fftW, 0.0));
	double maxMag = 0.0;

	for (int y = 0; y < fftH; ++y) {
		for (int x = 0; x < fftW; ++x) {
			double mag = std::abs(fftData[y][x]);
			magnitude[y][x] = std::log(1.0 + mag); // 고주파의 상세한 신호까지 보기 위해서 로그 스케일링 진행
			if (magnitude[y][x] > maxMag) maxMag = magnitude[y][x];
		}
	}

	// 영점(DC 성분)을 중앙으로 이동 (FFT Shift)
	CFFTEngine::ShiftFFT(magnitude, fftW, fftH);

	// FFT 결과용 CImage 생성 (24bit RGB)
	if (!m_fftImage.IsNull()) m_fftImage.Destroy();
	m_fftImage.Create(fftW, fftH, 24);

	// 0~255 정규화 후 CImage 픽셀 대입
	for (int y = 0; y < fftH; ++y) {
		for (int x = 0; x < fftW; ++x) {
			BYTE val = (maxMag > 0) ? static_cast<BYTE>((magnitude[y][x] / maxMag) * 255.0) : 0;
			m_fftImage.SetPixel(x, y, RGB(val, val, val));
		}
	}

	
	Invalidate(); // 화면 재그리기 -> OnDraw 호출
}
