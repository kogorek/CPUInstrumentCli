#include <fstream> // Сюда уже включен iostream
#include <iostream>
#include <ctime> // Необходимо для функции wait
#include <cstdio> // Необходима для sprintf
#include <unistd.h> // Необходимо для sleep()

void clearLine(int);

using namespace std;

// Класс ядра процессора.
class CPU
{
public:
    double f_maxFreq, f_minFreq, f_curFreq; // Переменные для макс мин и текущей частоты ядра.
    double f_scalingMaxFreq, f_scalingMinFreq; // Переменные для макс и мин частоты governor.
    int n_coreNumber; // Номер ядра.
    bool b_isOnline; // В сети ли ядро.

    CPU(int A)
    {
        n_coreNumber = A;
    }

private:
    void progressBar(double current, double min, double max)
    {
        // Progress bar version 0.5
        int percent = ((current-min)/(max-min))*100;
        // Вычисляем процент
        cout << percent << "% [";
        for (int i = 0; percent/5 >= i; i++)
            cout << '=';
        for (int i = 0; 19 - percent/5 >= i; i++)
            cout << ' ';
        cout << "]";
        // Результат работы: 50% [===========          ]
    }
    // Обновляем состояние ядра(в "сети" или нет)
    void updateOnline(void)
    {
        if (n_coreNumber != 0) //если ядро не основное (не 0)
        {
            char str_path[60]; // Переменная для пути к файлу
            sprintf(str_path,"%s%d%s", "/sys/devices/system/cpu/cpu", n_coreNumber, "/online\0");
            // Склейка пути
            ifstream fin; // Открываем файл и записываем в переменную
            fin.open(str_path);
            fin >> b_isOnline;
            fin.close();
        }
        else
            b_isOnline = 1;
    }
    // Обновляем максимальную частоту ядра
    void updateMaxFreq(void)
    {
        if (b_isOnline) //если ядро работает
        {
            char str_path[60]; // Переменная для пути к файлу
            sprintf(str_path,"%s%d%s", "/sys/devices/system/cpu/cpu", n_coreNumber, "/cpufreq/cpuinfo_max_freq\0");
            // Склейка пути
            ifstream fin; // Открываем файл и записываем в переменную
            fin.open(str_path);
            fin >> f_maxFreq;
            fin.close();
        }
        else
            f_maxFreq = 0;
    }
    // Обновляем минимальную частоту ядра
    void updateMinFreq(void)
    {
        if (b_isOnline) //если ядро работает
        {
            char str_path[60]; // Переменная для пути к файлу
            sprintf(str_path,"%s%d%s", "/sys/devices/system/cpu/cpu", n_coreNumber, "/cpufreq/cpuinfo_min_freq\0");
            // Склейка пути
            ifstream fin; // Открываем файл и записываем в переменную
            //fin.open(str_path);
            fin.open(str_path);
            fin >> f_minFreq;
            fin.close();
        }
        else
            f_minFreq = 0;
    }
    // Обновляем текущую частоту ядра
    void updateCurFreq(void)
    {
        if (b_isOnline) //если ядро работает
        {
            char str_path[60]; // Переменная для пути к файлу
            sprintf(str_path,"%s%d%s", "/sys/devices/system/cpu/cpu", n_coreNumber, "/cpufreq/scaling_cur_freq");
            // Склейка пути
            ifstream fin; // Открываем файл и записываем в переменную
            fin.open(str_path);
            fin >> f_curFreq;
            fin.close();
        }
        else
            f_curFreq = 0;
    }
    // Обновляем минимальную частоту ядра в governour
    void updateScalingMinFreq(void)
    {
        if (b_isOnline) //если ядро работает
        {
            char str_path[60]; // Переменная для пути к файлу
            sprintf(str_path,"%s%d%s", "/sys/devices/system/cpu/cpu", n_coreNumber, "/cpufreq/scaling_min_freq\0");
            // Склейка пути
            ifstream fin; // Открываем файл и записываем в переменную
            fin.open(str_path);
            fin >> f_scalingMinFreq;
            fin.close();
        }
        else
            f_scalingMinFreq = 0;
    }
    // Обновляем максимальную частоту ядра в governour
    void updateScalingMaxFreq(void)
    {
        if (b_isOnline) //если ядро работает
        {
            char str_path[60]; // Переменная для пути к файлу
            sprintf(str_path,"%s%d%s", "/sys/devices/system/cpu/cpu", n_coreNumber, "/cpufreq/scaling_max_freq\0");
            // Склейка пути
            ifstream fin; // Открываем файл и записываем в переменную
            fin.open(str_path);
            fin >> f_scalingMaxFreq;
            fin.close();
        }
        else
            f_scalingMaxFreq = 0;
    }
public:
    void updateInformation(void)
    {
        updateOnline();
        updateScalingMaxFreq();
        updateScalingMinFreq();
        updateCurFreq();
        updateMaxFreq();
        updateMinFreq();
    }
    void DEBUGprintInformation(void)
    {
        cout << "CPU" << n_coreNumber << " is online: " << b_isOnline << endl;
        cout << "CPU" << n_coreNumber << " maximum freq: " << f_maxFreq/1000 << "MHz" << endl;
        cout << "CPU" << n_coreNumber << " minimum freq: " << f_minFreq/1000 << "MHz" << endl;
        cout << "CPU" << n_coreNumber << " current freq: " << f_curFreq/1000 << "MHz" << endl;
        cout << "CPU" << n_coreNumber << " governor minimum freq: " << f_scalingMinFreq/1000 << "MHz" << endl;
        cout << "CPU" << n_coreNumber << " governor maximum freq: " << f_scalingMaxFreq/1000 << "MHz" << endl;
    }
    void drawProgressBar(void)
    {
        progressBar(f_curFreq, f_minFreq, f_maxFreq);
    }
    void printShit(void)
    {
        updateInformation();
        DEBUGprintInformation();
    }
};

//int main(int argc, char *argv[])
int main()
{
    //QCoreApplication a(argc, argv);
    int coreCount; // Переменная для количества ядер в системе
    coreCount = sysconf( _SC_NPROCESSORS_ONLN );
    CPU *cores[coreCount]; // Создаём указатель на массив указателей на объекты
    for (int i = 0; i < coreCount; i++)
        cores[i] = new CPU(i);
    for(int i = 0; i < coreCount; i++)
        cores[i]->updateInformation();
    while(true)
    {
        for(int i = 0; i < coreCount; i++)
        {
            cores[i]->updateInformation();
            cout << "CPU" << i << ": Min/Cur/Max ";
            cout << int(cores[i]->f_minFreq/1000) << "/"
                 << int(cores[i]->f_curFreq/1000) << "/"
                 << int(cores[i]->f_maxFreq/1000) << " MHz \t";
            cores[i]->drawProgressBar();
            cout << endl;
        }
        sleep(2);
        clearLine(coreCount);
    }
    delete [] *cores;
    return 0;
}
void clearLine(const int lines)
{
    for( int i = 1; i <= lines; i++ )
        cout << "\033[F\033[J";
}
