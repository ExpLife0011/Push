#include <windows.h>
#include <d3d8.h>

#include "d3d8font.h"
#include "d3d8overlay.h"
#include "d3d8hook.h"


Dx8Font* Dx8OvFont;
extern Dx8Overlay* OvDx8Overlay;


VOID IDirect3DDevice8_PresentCallback( 
    IDirect3DDevice8 *Device 
    )
{
    if (Dx8OvFont == NULL)
    {
        Dx8OvFont = new Dx8Font(Device);
        Dx8OvFont->RestoreDeviceObjects();

        GraphicsApi = API_D3D8;
    }

    OvDx8Overlay->Render();
}


VOID
IDirect3DDevice8_ResetCallback()
{
    if (Dx8OvFont)
    {
        Dx8OvFont->InvalidateDeviceObjects();
        Dx8OvFont->DeleteDeviceObjects();
    }

    Dx8OvFont = NULL;
}


Dx8Overlay::Dx8Overlay(
    OV_RENDER RenderFunction
    )
{
    HookD3D8( 
        IDirect3DDevice8_PresentCallback, 
        IDirect3DDevice8_ResetCallback 
        );

    UserRenderFunction = RenderFunction;
}


VOID
Dx8Overlay::DrawText( WCHAR* Text )
{
    DrawText(Text, FALSE);
}


VOID Dx8Overlay::DrawText( 
    WCHAR* Text, 
    DWORD Color 
    )
{
    DrawText(Text, 20, Line, Color);

    Line += 15;
}


VOID Dx8Overlay::DrawText( 
    WCHAR* Text, 
    int X, 
    int Y, 
    DWORD Color 
    )
{
    Dx8OvFont->DrawText(
        (FLOAT)X, 
        (FLOAT)Y, 
        Color, 
        Text, 
        NULL
        );
}


VOID
Dx8Overlay::Begin()
{

}


VOID
Dx8Overlay::End()
{

}


VOID*
Dx8Overlay::GetDevice()
{
    return NULL;
}

