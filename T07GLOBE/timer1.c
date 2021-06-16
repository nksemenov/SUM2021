/* Включение файла-заголовка функций работы со временем (для 'clock') */
/*#include <time.h>

double SyncTime;  /* время в секундах с момента старта программы */

/* Функция - "таймер" */
/*void Timer( void )
{
  long t = clock();
  static long StartTime = -1;

  if (StartTime == -1)
    StartTime = t;
  SyncTime = (t - StartTime) / (double)CLOCKS_PER_SEC;
} /* End of 'Timer' function */
/*необходимы:
DOUBLE GLB_Time       -- общее время в секундах
DOUBLE GLB_DeltaTime  -- межкадровое время в секундах
DOUBLE GLB_FPS        -- количество кадров в секунду
BOOL GLB_IsPause   -- флаг паузы	дополнительно запоминаем (в "тиках"-"клоках"):
LONG StartTime     -- время начала программы
LONG OldTime       -- время предыдущего кадра
LONG PauseTime     -- время простоя в паузе
LONG OldFPSTime    -- время предыдущего
                      замера FPS
LONG FrameCount    -- счетчик кадров для FPS

функционал:
  VOID GLB_TimerInit( VOID );      -- инициализация таймера
    StartTime = OldTime = OldFPSTime = clock();
    FrameCount = 0;
    GLB_IsPause = FALSE;
  VOID GLB_TimerResponse( VOID );  -- обновление таймера
    LONG t = clock();

    if (!GLB_IsPause)
    {
      GLB_Time = (DOUBLE)(t - PauseTime - StartTime) / CLOCKS_PER_SEC;
      GLB_DeltaTime = (DOUBLE)(t - OldTime) / CLOCKS_PER_SEC;
    }
    else
    {
      PauseTime += t - OldTime;
      GLB_DeltaTime = 0;
    }
    FrameCount++;
    if (t - OldFPSTime > CLOCKS_PER_SEC)
    {
      GLB_FPS = FrameCount / ((DOUBLE)(t - OldFPSTime) / CLOCKS_PER_SEC);
      OldFPSTime = t;
      FrameCount = 0;
    }
  OldTime = t;    
TIMER.C/TIMER.H*/
