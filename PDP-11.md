В PDP-11 байт состоит из 8 бит, а машинное слово из 16 бит, то есть 2 байт.
Архитектура, в которой младший разряд имеет наименьший адрес назвали little-endian или от младшего к старшему.

Размер памяти PDP-11 64 килобайта. То есть байты имеют адреса от 0 до 64 2^10 - 1 включительно. Не обязательно адрес - это натуральное число.

Слово в PDP-11 имеет тот же адрес, что его младшее полуслово (байт). То есть 0, 2, 4, 6 и так далее. В этом случае говорят, что слова и байты имеют единое адресное пространство.

1 бит (bit) - это 0 или 1.
1 байт (byte) - 8 бит в PDP-11.
1 машинное слово (word) - 16 бит в PDP-11.
В PDP-11 порядок байт little-endian.
Слова и байты имеют общее адресное пространство.
Адрес байта может быть любым, 0, 1, 2, 3, 4 и так далее;
Адрес слова только четный, 0, 2, 4, 6 и так далее.
