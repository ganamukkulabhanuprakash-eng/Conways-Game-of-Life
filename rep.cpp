#include <windows.h>
#include <vector>
#include <ctime>

const int CELL_SIZE = 10;
const int ROWS = 40;
const int COLS = 60;

std::vector<std::vector<bool>> grid(ROWS, std::vector<bool>(COLS, false));
std::vector<std::vector<bool>> nextGrid(ROWS, std::vector<bool>(COLS, false));

int countNeighbors(int r, int c) {
    int liveNeighbors = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int ni = r + i, nj = c + j;
            if (ni >= 0 && ni < ROWS && nj >= 0 && nj < COLS) {
                if (grid[ni][nj]) liveNeighbors++;
            }
        }
    }
    return liveNeighbors;
}

void updateGrid() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            int neighbors = countNeighbors(i, j);
            if (grid[i][j]) {
                nextGrid[i][j] = (neighbors == 2 || neighbors == 3);
            } else {
                nextGrid[i][j] = (neighbors == 3);
            }
        }
    }
    grid = nextGrid;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_TIMER:
            updateGrid();
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Create double buffer to stop screen flickering
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hbmMem = CreateCompatibleBitmap(hdc, COLS * CELL_SIZE, ROWS * CELL_SIZE);
            SelectObject(hdcMem, hbmMem);

            // Dark Grey Background
            HBRUSH bgBrush = CreateSolidBrush(RGB(20, 20, 20));
            RECT rect = {0, 0, COLS * CELL_SIZE, ROWS * CELL_SIZE};
            FillRect(hdcMem, &rect, bgBrush);
            DeleteObject(bgBrush);

            // Green Live Cells
            HBRUSH cellBrush = CreateSolidBrush(RGB(46, 204, 113));
            for (int i = 0; i < ROWS; ++i) {
                for (int j = 0; j < COLS; ++j) {
                    if (grid[i][j]) {
                        RECT cellRect = { j * CELL_SIZE, i * CELL_SIZE, (j + 1) * CELL_SIZE - 1, (i + 1) * CELL_SIZE - 1 };
                        FillRect(hdcMem, &cellRect, cellBrush);
                    }
                }
            }
            DeleteObject(cellBrush);

            // Copy to screen
            BitBlt(hdc, 0, 0, COLS * CELL_SIZE, ROWS * CELL_SIZE, hdcMem, 0, 0, SRCCOPY);
            DeleteObject(hbmMem);
            DeleteDC(hdcMem);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    srand(time(0));
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < COLS; ++j)
            grid[i][j] = (rand() % 5 == 0);

    const char CLASS_NAME[] = "GameOfLifeWindow";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "Conway's Game of Life (Native Windows)", 
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, 
        CW_USEDEFAULT, CW_USEDEFAULT, COLS * CELL_SIZE + 16, ROWS * CELL_SIZE + 39, 
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);
    SetTimer(hwnd, 1, 100, NULL); // Updates window every 100ms

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}