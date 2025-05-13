# Analysis-of-string-sorts
## Случайные строки (random)
Время:
![Figure_1](https://github.com/user-attachments/assets/ed4ca0d9-41d5-499a-bbba-0cee4d378848)
Сравнения:
![Figure_2](https://github.com/user-attachments/assets/c5c3bc67-ddc8-4eb1-a17d-0e5b379beb68)
MSD Radix Sort оказался самым медленным, несмотря на отсутствие сравнений (сортирует по байтам).
String QuickSort и Hybrid — одни из самых быстрых.
Std MergeSort предсказуемо медленнее Std QuickSort.

### Обратно отсортированные строки (reverse_sorted)
Время:
![Figure_3](https://github.com/user-attachments/assets/3c06b540-9801-42f6-b1c2-2312ca942844)
Сравнения:
![Figure_4](https://github.com/user-attachments/assets/30895215-2076-48a2-b8a3-fcd5605093ae)
String QuickSort сильно деградирует по количеству сравнений.
MSD Radix Sort стабилен, но по-прежнему не быстр.
Hybrid Radix — один из лучших вариантов по сбалансированности.

## Почти отсортированные (almost_sorted)
Время:
![Figure_5](https://github.com/user-attachments/assets/cdaa1879-d3c0-4feb-82a4-5262183bb6ac)
Сравнения:
![Figure_6](https://github.com/user-attachments/assets/a1f77d9c-777d-44d1-8040-ec927b325927)
Почти все алгоритмы показывают плавный линейный рост времени, без резких скачков.
Std QuickSort чуть хуже адаптирован к почти отсортированным данным, чем MergeSort.
String QuickSort производит больше сравнений, чем нужно, из-за повторного прохода по префиксам.

## Строки с общим префиксом (common_prefix)
Время:
![Figure_7](https://github.com/user-attachments/assets/7a58dc7c-68f8-4ef5-a08b-af23b689b3d5)
Сравнения:
![Figure_8](https://github.com/user-attachments/assets/02c90d80-927c-428e-9530-98b55e922ff6)
У String MergeSort максимальное количество сравнений: алгоритм проводит глубокий анализ каждого префикса.
Hybrid и Std QuickSort — эффективны по времени.
MSD снова стабилен, но не лидирует по скорости.





