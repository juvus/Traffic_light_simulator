/* =================================================================================== */
/* Traffic lights simulation programm                                                  */
/* Win32 Platform for windows creation and utilities                                   */
/* =================================================================================== */

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include <stdio.h> /* for testing purposes */

#include <utils.h>
#include <simulation.h>
#include <resource.h>

// Macro to suppress "unused function parameter"
#define UNUSED(x) (void)(x)

/* Window dimensions */
#define WINDOW_WIDTH 220
#define WINDOW_HEIGHT 600

/* Timer parameters */
#define IDT_TIMER1 101
static UINT TIME_INTERVAL = 10; /* Produce about */

/* Global variables */
static b32 running = true; /* State of the whole simulation */
static Render_Buffer_t render_buffer; /* Buffer for the whole window image */
static LARGE_INTEGER begin_counter;
static LARGE_INTEGER end_counter;
static LARGE_INTEGER frequency_counter;
static f32 dtime = 0.01666f; /* 60 fps */
static RECT rect;
static MSG message;
static HDC hdc = NULL;
static Key_Pressed_t key_pressed = KP_NONE;

/* Callback function */
static LRESULT CALLBACK
window_callback (HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) {

    LRESULT result = 0;

    switch (message) {
    case WM_CLOSE:
    case WM_DESTROY: {
        running = false;
    } break;
        
    case WM_CREATE: {
        /* Retrieves the coordinates of a window's client area */
        GetClientRect(hwnd, &rect);
        
        /* Determine dimensions of the client area */
        render_buffer.width = (u32)(rect.right - rect.left);
        render_buffer.height = (u32)(rect.bottom - rect.top);
        
        /* Memory allocation with windows layer VirtualAlloc function */
        render_buffer.pixels = VirtualAlloc(0, sizeof(u32) * render_buffer.width * render_buffer.height,
                                            MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
        
        /* Fill the bitmap_info */
        render_buffer.bitmap_info.bmiHeader.biSize = sizeof(render_buffer.bitmap_info.bmiHeader);
        render_buffer.bitmap_info.bmiHeader.biWidth = (LONG)render_buffer.width;
        render_buffer.bitmap_info.bmiHeader.biHeight = (LONG)render_buffer.height;
        render_buffer.bitmap_info.bmiHeader.biPlanes = 1;
        render_buffer.bitmap_info.bmiHeader.biBitCount = 32;
        render_buffer.bitmap_info.bmiHeader.biCompression = BI_RGB;
    } break;
        
    case WM_TIMER: {
        /* Process the timer message */
        switch (w_param) {
        case IDT_TIMER1: {

            /* Make the step of the simulation */
            simulate_traffic_light(dtime, key_pressed, &render_buffer, &running);
            
            /* Reset the key_pressed variable  */
            key_pressed = KP_NONE;
            
            /* Render the calculated bitmap */
            StretchDIBits(hdc, 0, 0, (int)render_buffer.width, (int)render_buffer.height, 0, 0,
                          (int)render_buffer.width, (int)render_buffer.height, render_buffer.pixels,
                          &render_buffer.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

            /* Calculate the delta time */
            QueryPerformanceCounter(&end_counter);
            dtime = (f32)((end_counter.QuadPart - begin_counter.QuadPart) / (f32)frequency_counter.QuadPart);
            begin_counter = end_counter;
            
            break;
        }}
    } break;   

    case WM_COMMAND: {
        /* Precess the commands defined in acceleration table (key pressed events) */
        switch (LOWORD(w_param)) {
        case ID_AUTO_WORK_MODE: {
            /* Ctrl + A was pressed */
            key_pressed = KP_AUTO_WORK_MODE;
            break;
        }
        case ID_MANUAL_WORK_MODE: {
            /* Ctrl + M was pressed */
            key_pressed = KP_MANUAL_WORK_MODE;
            break;
        }
        case ID_TOGGLE_GREEN: {
            /* F1 was pressed */
            key_pressed = KP_TOGGLE_GREEN;
            break;
        }
        case ID_TOGGLE_YELLOW: {
            /* F2 was pressed */
            key_pressed = KP_TOGGLE_YELLOW;
            break;
        }
        case ID_TOGGLE_RED: {
            /* F3 was pressed */
            key_pressed = KP_TOGGLE_RED;
            break;
        }
        case ID_ESCAPE: {
            /* Escape was pressed */
            key_pressed = KP_ESCAPE;
        }}
    } break;
        
    default: {
        result = DefWindowProcA(hwnd, message, w_param, l_param);
    }}
    return result;
}

/* Entry point for the windows application */
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    /* Avoid warning messages about not used function parameters */
    UNUSED(hPrevInstance);
    UNUSED(lpCmdLine);
    UNUSED(nShowCmd);

    /* Load the accelerator table from resources */
    HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(ID_ACCEL1));
    
    /* Create a window class */
    WNDCLASSA window_class = {0};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = window_callback;
    window_class.lpszClassName = "Simulation";

    /* Register the window class */
    RegisterClassA(&window_class);

    /* Create a window and return a window handle */
    DWORD dwStyle = WS_VISIBLE | WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU;
    HWND hwnd = CreateWindowExA(0, window_class.lpszClassName, "Simulation", dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0);

    // Check if the window was successfully created
    if (hwnd == NULL) {
        assert(0 && "Window was not successfully created");
    }
      
    /* Get the drawing content of the out created window */
    hdc = GetDC(hwnd);

    /* Set the timer */
    SetTimer(hwnd, IDT_TIMER1, TIME_INTERVAL, (TIMERPROC)NULL);
    
    /* Window main loop */
    QueryPerformanceFrequency(&frequency_counter);
    QueryPerformanceCounter(&begin_counter);

    /* Application loop */   
        /* Threat the messages */
    while ((GetMessage(&message, NULL, 0, 0)) && running) {
        if (!TranslateAccelerator(hwnd, hAccel, &message)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }

    /* Free the memory */
    VirtualFree(render_buffer.pixels, 0, MEM_RELEASE);
    return 0;
}
