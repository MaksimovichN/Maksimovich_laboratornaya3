// MsgClient.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include "framework.h"
#include "MsgClient.h"
#include "../MsgServer/Msg.h"
#include <mutex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

mutex m1;

void ReceiveMsg()
{
    while (1) {
        Sleep(1000);
        Message msg = Message::Send(M_BROKER, M_GETDATA);
        if (msg.m_Header.m_Type == M_DATA)
            cout << endl << "Received Msg From " << msg.m_Header.m_From << ": " << msg.m_Data;
    }
}


void Client()
{
    setlocale(LC_ALL, "Russian");
    AfxSocketInit();
    Message msg = Message::Send(M_BROKER, M_INIT);
    thread tt(ReceiveMsg);
    tt.detach();
    bool check;
    while (1) {
        cout << "Ввести сообщение - 0; Выйти - 1: ";
        cin >> check;
        switch (check)
        {
        case 0:
        {
            cout << "Личное - 0; Широковещательное - 1: ";
            cin >> check;
            cout << "Введите сообщение: ";
            cin >> msg.m_Data;
            switch (check) {
            case 0:
            {
                cout << "Введите получателя: ";
                cin >> msg.m_Header.m_To;
                Message::Send(msg.m_Header.m_To, M_DATA, msg.m_Data);
                break;
            }
            case 1:
            {
                Message::Send(M_ALL, M_DATA, msg.m_Data);
                break;
            }
            }
            break;
        }
        case 1:
        {
            Message::Send(M_BROKER, M_EXIT);
            return;
        }
        }
    }
}

// Единственный объект приложения

CWinApp theApp;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // инициализировать MFC, а также печать и сообщения об ошибках про сбое
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: вставьте сюда код для приложения.
            wprintf(L"Критическая ошибка: сбой при инициализации MFC\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: вставьте сюда код для приложения.
            Client();
        }
    }
    else
    {
        // TODO: измените код ошибки в соответствии с потребностями
        wprintf(L"Критическая ошибка: сбой GetModuleHandle\n");
        nRetCode = 1;
    }

    return nRetCode;
}
