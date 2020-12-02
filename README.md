# fractal-image-encoding

Реализован алгоритм фрактального представления трех полутоновых изображений: Goldhill.bmp, Lena-gs.bmp, Boat.bmp.

Изображение разбивается на блоки по 4 пикселя. Сдвиг при поиске “похожего” блока – 4 пикселя. Сравнение блоков производилось по метрике L1. Для декодирования каждого изображения использовалось 12 итераций. Применялись дополнительная классификация блоков (16 классов), дополнительное разбиение блоков на меньшие при слишком большом показателе по метрике L1 (>70).
Время обработки одного изображения – примерно 5 минут.

![Иллюстрация к проекту](https://github.com/natalia-va/fractal-image-encoding/blob/main/img/Goldhill.bmp)
