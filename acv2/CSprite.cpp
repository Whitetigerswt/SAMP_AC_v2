#include "CSprite.h"
#include "Addresses.h"

Sprite::Sprite()
{
	pos.x = 0;
	pos.y = 0;

	color = D3DCOLOR_ARGB(255, 255, 255, 255);

	initialized = false;

}

Sprite::Sprite(float x, float y)
{
	pos.x = x;
	pos.y = y;

	scale = D3DXVECTOR2();

	color = D3DCOLOR_ARGB(255, 255, 255, 255);

	initialized = false;
}

Sprite::~Sprite()
{
	if (sprite)
	{
		sprite->Release();
		sprite = NULL;
	}

	if (tex)
	{
		tex->Release();
		tex = NULL;
	}

	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}

	initialized = false;

}

bool Sprite::Init(LPDIRECT3DDEVICE9 device, float width, float height)
{

	if (device == NULL || initialized == true) return false;

	m_device = device;

	center = D3DXVECTOR2(width / 2, height / 2);
	scale = D3DXVECTOR2(width, height);

	if (FAILED(D3DXCreateTextureFromResourceEx(device, GetModuleHandle("ACv2_Client.asi"), MAKEINTRESOURCE(IDB_PNG1), int(width), int(height), D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &tex)))
	{
		return false;
	}

	if (FAILED(D3DXCreateSprite(device, &sprite)))
	{
		return false;
	}

	SetTransformation();

	m_CurrAlpha = 0;
	m_ColorTick = 0;
	m_FadeType = false;
	m_Type = 0;

	D3DXCreateFont(m_device, 10, 8, 1200, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Verdana", &m_pFont);

	initialized = true;

	return true;
}

bool Sprite::isInitialized()
{
	return initialized;
}

void Sprite::Draw()
{
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	if (m_Type == 0)
	{
		if (sprite && tex)
		{
			sprite->Draw(tex, NULL, NULL, &D3DXVECTOR3(pos.x, pos.y, 0), color);

			m_ColorTick++;

			if (m_ColorTick == 3)
			{
				color = D3DCOLOR_ARGB(m_CurrAlpha, 255, 255, 255);
				m_ColorTick = 0;
				if (m_FadeType == false)
					m_CurrAlpha++;
				else
					m_CurrAlpha--;

				if (m_CurrAlpha == 0)
				{
					m_FadeType = false;
					m_Type = 1;
				}
				else if (m_CurrAlpha == 255)
				{
					m_FadeType = true;
				}
			}
		}
	}
	else if (m_Type == 1)
	{
		if (m_pFont)
		{
			RECT rect;
			rect.bottom = 600;
			rect.top = 500;
			rect.left = 100;
			rect.right = 500;

			if (!m_FadeType)
			{
				m_CurrAlpha++;
			}
			else
			{
				m_CurrAlpha--;
			}

			m_pFont->DrawText(sprite, "SA:MP AC created by: Whitetiger", -1, &rect, DT_SINGLELINE | DT_LEFT, D3DCOLOR_ARGB(m_CurrAlpha, 255, 255, 255));

			if (m_CurrAlpha == 255)
			{
				m_FadeType = true;
			}
			else if (m_CurrAlpha == 0)
			{
				m_FadeType = false;
				m_Type = 2;
			}
		}
	}
	else if (m_Type == 2)
	{
		if (m_pFont)
		{
			RECT rect;
			rect.bottom = 600;
			rect.top = 500;
			rect.left = 100;
			rect.right = 500;

			if (!m_FadeType)
			{
				m_CurrAlpha++;
			}
			else
			{
				m_CurrAlpha--;
			}

			m_pFont->DrawText(sprite, "www.github.com/whitetigerswt", -1, &rect, DT_SINGLELINE | DT_LEFT, D3DCOLOR_ARGB(m_CurrAlpha, 255, 255, 255));

			if (m_CurrAlpha == 255)
			{
				m_FadeType = true;
			}
			else if (m_CurrAlpha == 0)
			{
				m_Type = 3;
				// enable for pause menu, somehow don't fuck up resolution changes.
			}
		}
	}

	sprite->End();
}


void Sprite::Reset()
{
	HRESULT r = m_device->TestCooperativeLevel();

	if (r == D3DERR_DEVICELOST)
		OnLostDevice();

	else if (r == D3DERR_DEVICENOTRESET)
		OnResetDevice();
}

void Sprite::OnLostDevice()
{
	initialized = false;
	delete this;
}

void Sprite::OnResetDevice()
{
	initialized = false;
	delete this;
}

void Sprite::Resize(float width, float height)
{
	scale = D3DXVECTOR2(width, height);
}

void Sprite::Move(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

void Sprite::Rotate(float angle)
{
	rotation = angle;
}

void Sprite::SetTransformation()
{
	D3DXVECTOR2 spriteCenter = center;

	D3DXVECTOR2 trans = pos;

	D3DXVECTOR2 scaling = scale;

	D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &spriteCenter, rotation, &trans);
}