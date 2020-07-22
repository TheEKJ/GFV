#include "imagedrawable.h"
#include "global.h"

#include <assert.h>

ImageDrawable::ImageDrawable(const wchar_t* wImage, const IDRAW_TYPE_LOAD& typeLoad, const PointF& position) : m_bitmapDecoder(nullptr),
m_bitmapFrameDecode(nullptr),
m_formatConverter(nullptr),
m_bitmap(nullptr),
m_scale(1),
m_opacity(1.f),
m_typeLoad(typeLoad)
{
	assert(init(wImage));
}

bool ImageDrawable::init(const wchar_t* wImage)
{
	SafeRelease(m_bitmapDecoder);
	SafeRelease(m_bitmapFrameDecode);
	SafeRelease(m_formatConverter);
	SafeRelease(m_bitmap);

	HRESULT hr;

	switch (m_typeLoad)
	{
	case FILEPATH:
		hr = global_WICImagingFactory->CreateDecoderFromFilename(wImage, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &m_bitmapDecoder);
		break;
	case RESOURCE:
	{
		HRSRC rscHandle = NULL;
		HGLOBAL rscData = NULL;
		LPVOID rscFile = NULL;
		DWORD rscFileSize = NULL;
		IWICStream* stream;

		rscHandle = FindResource(nullptr, wImage, L"IMAGE");
		assert(rscHandle);

		rscData = LoadResource(nullptr, rscHandle);

		rscFile = LockResource(rscData);

		rscFileSize = SizeofResource(nullptr, rscHandle);

		hr = global_WICImagingFactory->CreateStream(&stream);

		if(SUCCEEDED(hr))hr = stream->InitializeFromMemory(reinterpret_cast<WICInProcPointer>(rscFile), rscFileSize);
		
		if(SUCCEEDED(hr))hr = global_WICImagingFactory->CreateDecoderFromStream(stream, NULL, WICDecodeMetadataCacheOnLoad, &m_bitmapDecoder);
	}
		break;
	default:
		assert("Error: IDRAW_TYPE_LOAD not specified!");
		break;
	}


	if(SUCCEEDED(hr))hr = m_bitmapDecoder->GetFrame(0, &m_bitmapFrameDecode);
	if (SUCCEEDED(hr))hr = global_WICImagingFactory->CreateFormatConverter(&m_formatConverter);
	if (SUCCEEDED(hr))hr = m_formatConverter->Initialize(m_bitmapFrameDecode, GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
	if (SUCCEEDED(hr))hr = m_bitmapFrameDecode->GetSize(&m_size.width, &m_size.height);
	

	m_init = false;

	return SUCCEEDED(hr) ? true : false;
}

void ImageDrawable::Draw(Graphics* source)
{
	if (!m_init)
	{
		assert(SUCCEEDED(source->GetRenderTarget()->CreateBitmapFromWicBitmap(m_formatConverter, &m_bitmap)));

		SafeRelease(m_bitmapDecoder);
		SafeRelease(m_bitmapFrameDecode);
		SafeRelease(m_formatConverter);

		m_init = true;
	}

	source->GetRenderTarget()->DrawBitmap(m_bitmap, D2D1::RectF(m_position.x,
		m_position.y,
		m_position.x + m_size.width * m_scale.x,
		m_position.y + m_size.height * m_scale.y),
		m_opacity,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}

/*
PROPVARIANT propValue;
PropVariantInit(&propValue);

IWICMetadataBlockReader* metadataBlockReader = nullptr;
IWICMetadataReader* metadataReader = nullptr;
HRESULT hr = m_bitmapFrameDecode->QueryInterface(IID_PPV_ARGS(&metadataBlockReader));

assert(SUCCEEDED(hr));

UINT total;
hr = metadataBlockReader->GetCount(&total);

if (SUCCEEDED(hr))
{
	for (UINT i = 0; i < total; i++)
	{
		metadataBlockReader->GetReaderByIndex(i, &metadataReader);
		UINT subtotal;
		metadataReader->GetCount(&subtotal);

		for (UINT j = 0; j < subtotal; j++)
		{
			PROPVARIANT id,value;
			PropVariantInit(&id);
			PropVariantInit(&value);

			hr = metadataReader->GetValueByIndex(j, NULL, &id, &value);
			if (SUCCEEDED(hr))
			{
				if(id.pwszVal != nullptr)
					std::wcout << id.pwszVal << ":\t"<<value.uiVal<<std::endl;
			}

			PropVariantClear(&id);
			PropVariantClear(&value);
		}
		metadataReader->Release();
	}
}

PropVariantClear(&propValue);
metadataBlockReader->Release();*/