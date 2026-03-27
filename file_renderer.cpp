#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "Point3D.h"
#include "CNUMPP_H.h"
#include "STLREADER_H.h"
#include "BITMAP_H.h"

// Globals for the Win32 window
static Bitmap *g_bitmap = nullptr;
static std::vector<std::array<Vector3D, 4>> *g_stl_data = nullptr;
static float g_rotationY = 0.0f;
static float g_rotationX = 0.0f;
static bool g_running = true;
static BITMAPINFO g_bmi = {};

void renderFrame(Bitmap &bitmap, const std::vector<std::array<Vector3D, 4>> &stl_data,
                 float rotY, float rotX)
{
    bitmap.clear();

    Vector3D light_vector = Vector3D(-0.1f, -0.2f, -1.0f);

    for (const auto &face : stl_data)
    {
        float vertex_array[3][4] =
            {{face[1].x, face[1].y, face[1].z, 1},
             {face[2].x, face[2].y, face[2].z, 1},
             {face[3].x, face[3].y, face[3].z, 1}};

        Vector3D facet_vector = Vector3D(face[0].x, face[0].y, face[0].z);

        // Apply rotations
        facetRotationX(*vertex_array, 3, (int)rotX, facet_vector);
        facetRotationY(*vertex_array, 3, (int)rotY, facet_vector);

        float shade = dotproduct(facet_vector, light_vector) / 3.14159f;
        shade = 1.0f - shade;
        shade = std::clamp(shade, 0.0f, 1.0f);

        Vector3D point1 = Vector3D(vertex_array[0][0], vertex_array[0][1], vertex_array[0][2]);
        Vector3D point2 = Vector3D(vertex_array[1][0], vertex_array[1][1], vertex_array[1][2]);
        Vector3D point3 = Vector3D(vertex_array[2][0], vertex_array[2][1], vertex_array[2][2]);

        bitmap.drawallFacet(point1, point2, point3, (uint8_t)(255 * shade), 0, 0);
        bitmap.drawFacetedge(point1, point2, point3, 255, 255, 255);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        g_running = false;
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            g_running = false;
            PostQuitMessage(0);
        }
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        if (g_bitmap)
        {
            // Blit the bitmap buffer to the window
            // Our buffer is top-to-bottom, so use negative height to flip
            StretchDIBits(hdc,
                          0, 0, g_bitmap->getWidth(), g_bitmap->getHeight(),
                          0, 0, g_bitmap->getWidth(), g_bitmap->getHeight(),
                          g_bitmap->getData(), &g_bmi, DIB_RGB_COLORS, SRCCOPY);
        }
        EndPaint(hwnd, &ps);
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main()
{
    const int W = 800;
    const int H = 800;

    Bitmap bitmap(W, H);
    g_bitmap = &bitmap;

    // Load STL
    std::string stlFilePath = ".stl_files/Dodecahedron-converted-ASCII.stl";
    std::string stl_string = readFileIntoString(stlFilePath);
    if (stl_string.empty())
    {
        std::cerr << "Failed to load STL file: " << stlFilePath << std::endl;
        return 1;
    }

    std::vector<std::array<Vector3D, 4>> stl_data;
    parseSTL(stl_string, stl_data);
    g_stl_data = &stl_data;

    std::cout << "Loaded " << stl_data.size() << " facets" << std::endl;

    // Setup BITMAPINFO for StretchDIBits
    // Use negative height so the DIB is top-down (matches our buffer layout)
    memset(&g_bmi, 0, sizeof(g_bmi));
    g_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    g_bmi.bmiHeader.biWidth = W;
    g_bmi.bmiHeader.biHeight = -H; // negative = top-down
    g_bmi.bmiHeader.biPlanes = 1;
    g_bmi.bmiHeader.biBitCount = 24;
    g_bmi.bmiHeader.biCompression = BI_RGB;

    // Create window
    const char CLASS_NAME[] = "3DRendererWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    // Calculate window size to get exact client area
    RECT rect = {0, 0, W, H};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "3D Renderer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!hwnd)
    {
        std::cerr << "Failed to create window" << std::endl;
        return 1;
    }

    ShowWindow(hwnd, SW_SHOW);

    // Main loop: rotate and render continuously
    MSG msg = {};
    while (g_running)
    {
        // Process all pending messages
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                g_running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!g_running)
            break;

        // Animate rotation
        g_rotationY += 1.0f;
        if (g_rotationY >= 360.0f)
            g_rotationY -= 360.0f;

        // Render to bitmap
        renderFrame(bitmap, stl_data, g_rotationY, g_rotationX);

        // Redraw the window
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);

        // ~60fps
        Sleep(16);
    }

    // Also save final frame to file
    bitmap.save("output.bmp");

    return 0;
}
