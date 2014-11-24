#include "CSprite.h"
#include "CLog.h"

Sprite::Sprite()
{
	pos.x = 0;
	pos.y = 0;

	color = D3DCOLOR_ARGB(255, 255, 255, 255);

	initialized = false;

}

Sprite::Sprite(int x, int y)
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

	initialized = false;

}

bool Sprite::Init(LPDIRECT3DDEVICE9 device, int width, int height)
{
	m_device = device;

	center = D3DXVECTOR2((width / 2), (height / 2));
	scale = D3DXVECTOR2(width, height);

	HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(IDB_PNG1), RT_RCDATA);

	HGLOBAL hData = LoadResource(NULL, hRes);
	LPVOID data = LockResource(hData);

	CLog log = CLog("testa.txt");

	if (FAILED(D3DXCreateTextureFromResourceEx(device, GetModuleHandle("ACv2_Client.asi"), MAKEINTRESOURCE(IDB_PNG1), width, height, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &tex)))
	{
		log.Write("first failed");
		return false;
	}

	if (FAILED(D3DXCreateSprite(device, &sprite)))
	{
		log.Write("second failed");
		return false;
	}

	SetTransformation();

	initialized = true;
	
	return true;
}

bool Sprite::isInitialized()
{
	return initialized;
}

void Sprite::Draw()
{
	if (sprite && tex)
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		sprite->Draw(tex, NULL, NULL, &D3DXVECTOR3(pos.x, pos.y, 0), color);
		sprite->End();
	}
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
	sprite->OnLostDevice();
}

void Sprite::OnResetDevice()
{
	sprite->OnResetDevice();
}

void Sprite::Resize(int width, int height)
{
	scale = D3DXVECTOR2(width, height);
}

void Sprite::Move(int x, int y)
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