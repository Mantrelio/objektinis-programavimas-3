# Studentų valdymo programa

Tai C++17 projektas studentų duomenims nuskaityti, generuoti, rūšiuoti, skaidyti į dvi grupes ir išvesti į ekraną arba į failą. Programa palaiko kelis konteinerių tipus: `vector`, `list` ir `deque`.

## Funkcionalumas

- studentų įrašų nuskaitymas iš failo
- studentų kūrimas rankiniu būdu arba generuojant atsitiktinius duomenis
- galutinio pažymio skaičiavimas pagal vidurkį arba medianą
- studentų rūšiavimas pagal vardą, pavardę arba galutinį pažymį
- studentų skaidymas į `nuskriausti` ir `protingi`
- rezultatų išvedimas į ekraną arba į tekstinį failą
- testai didelėms duomenų imtims ir skirtingiems konteineriams

## Sistemos specifikacijos

Projektas buvo testuotas šioje aplinkoje:

| Paskirtis | Reikšmė |
| --- | --- |
| Operacinė sistema | Ubuntu 24.04.4 LTS |
| Branduolys | 6.17.0-20-generic |
| Kompiliatorius | g++ 13.3.0 |
| C++ standartas | C++17 |
| Procesorius | Intel Core i5-1035G1 @ 1.00 GHz |
| Operatyvioji atmintis | 7.3 GiB |
| Saugykla | SAMSUNG MZALQ256HAJD-000L2 NVMe SSD, 238.5 GB |

## Reikalavimai

- `g++` su C++17 palaikymu
- `make`

Greita patikra Linux aplinkoje:

```bash
g++ --version
make --version
```

## Kompiliavimas

1. Pereikite į projekto aplanką.
2. Surinkite programą su `make`.

```bash
cd /home/mantrimas/Documents/OOP/objektinis-programavimas
make
```

Tai sukuria vykdomąjį failą `student-vector`.

## Paleidimas

Paleiskite programą:

```bash
./student-vector
```

Išvalymas (jei reikia pilno perkompiliavimo):

```bash
make clean
```

Pilnas perkompiliavimas:

```bash
make clean && make
```

## Kaip paleisti našumo testus

Interaktyviame meniu pasirinkite:

1. `8 - Testuoti studentu skirstyma i nuskriaustus ir protingus`
2. Konteinerį:
	`1 - vector<Student>`
	`2 - list<Student>`
	`3 - deque<Student>`
	`4 - CustomVector<Student>`
3. Strategiją:
	`1 - Pirma`
	`2 - Antra`
	`3 - Trecia`

Testai paleidžiami failams:

- `students1000`
- `students10000`
- `students100000`
- `students1000000`
- `students10000000`

Programa pateikia laikus šioms dalims:

- failo nuskaitymas
- rūšiavimas
- skirstymas į `nuskriausti` ir `protingi`
- bendras laikas

## `Student` klasė: Rule of Five ir operatoriai

`Student` klasė paveldi abstrakčią bazinę klasę `Human` (failas `include/human.h`).

`Human` klasė apibrėžia:

- bendrus laukus `name_` ir `surname_`
- bendrus metodus `name()`, `surname()`, `setName(...)`, `setSurname(...)`
- virtualų destruktorių `virtual ~Human() = 0`, kuris leidžia saugiai naudoti polimorfizmą

Taip `Student` klasėje nelieka dubliuojamų vardo ir pavardės laukų, o paveldėti metodai naudojami tiesiogiai visame projekte.

### `Human` abstraktumo iliustracija

Žemiau pateikta iliustracija parodo, kad `Human` klasės objekto sukurti negalima, nes klasė yra abstrakti.

![Human abstraktumo klaida](assets/human-abstract-failure.png)

`Student` klasėje (failas `include/student.h`) yra realizuoti šie Rule of Five metodai:

- kopijavimo konstruktorius `Student(const Student& other)`
- perkėlimo konstruktorius `Student(Student&& other) noexcept`
- kopijavimo priskyrimo operatorius `Student& operator=(const Student& other)`
- perkėlimo priskyrimo operatorius `Student& operator=(Student&& other) noexcept`
- destruktorius `~Student()`

Papildomai realizuoti įvedimo/išvedimo operatoriai:

- `operator>>(std::istream&, Student&)`:
  - nuskaito vieną eilutę formatu `Vardas Pavardė ND... Egzaminas`
  - paskutinis skaičius laikomas egzamino pažymiu
  - tikrina pažymių intervalą `[1..10]`
  - neteisingos įvesties atveju nustato `failbit`
- `operator<<(std::ostream&, const Student&)`:
  - išveda vardą, pavardę, galutinį balą pagal vidurkį ir medianą
  - taiko lygiuotę, stulpelių plotį ir 2 skaitmenų tikslumą po kablelio

### Šių dalių testų padengimas

Projekte yra du testavimo keliai:

1. Interaktyvūs runtime testai meniu punkte `9 - Testuoti Student Rule of Five ir operatorius` (failas `src/test/runtime-test/student-rule-of-five-operators-test.cpp`).
2. Catch2 unit testai (failas `src/test/unit-test/student-catch2-test.cpp`).

Įgyvendinti unit testai padengia:

- kopijavimo konstruktorių
- perkėlimo konstruktorių
- kopijavimo priskyrimo operatorių
- perkėlimo priskyrimo operatorių
- destruktoriaus saugų panaudojimą (sunaikinimas ir objekto atstatymas toje pačioje atmintyje)
- `operator>>` su korektiška įvestimi
- `operator>>` su nekorektiška įvestimi (`failbit`)
- `operator<<` formatavimą (lyginama pilna suformatuota eilutė)
- `Human` abstraktumo tikrinimą (`std::is_abstract`, `std::is_default_constructible`)

### Kaip sukompiliuoti ir paleisti unit testus

Iš projekto šakninio aplanko:

```bash
make test
./test
```

Papildomai galima paleisti tik pasirinktus Catch2 testus pagal žymes (tags):

```bash
./test "[rule-of-five]"
./test "[operators]"
```

## `CustomVector` klasė

`CustomVector<T>` yra projekte naudojamas dinaminis masyvas (šabloninė klasė failuose `include/custom-vector.h` ir `include/custom-vector.tpp`). Viduje elementai saugomi `std::unique_ptr<T[]>` buferyje; seka `size_` (elementų skaičius) ir `capacity_` (rezervuota vieta).

### Pagalbinės funkcijos (naudojamos viduje)

`begin()` — žymeklis į pirmą elementą:

```cpp
T* CustomVector<T>::begin() noexcept {
    return data_.get();
}
```

`end()` — žymeklis už paskutinio elemento (įterpimas į galą naudoja `end()`):

```cpp
T* CustomVector<T>::end() noexcept {
    return data_.get() + size_;
}
```

`max_size()` — viršutinė elementų riba prieš `reserve` / `insert`:

```cpp
typename CustomVector<T>::size_type CustomVector<T>::max_size() const noexcept {
    return (std::min)(std::numeric_limits<size_type>::max(),
                      static_cast<size_type>(std::numeric_limits<std::ptrdiff_t>::max())) /
           sizeof(T);
}
```

`reset()` (private) — atlaisvina buferį ir nulinėja `size_`, `capacity_`:

```cpp
void CustomVector<T>::reset() noexcept {
    data_.reset();
    size_ = 0;
    capacity_ = 0;
}
```

### `push_back(const T& value)`

Prideda elemento kopiją į konteinerio galą: elementai neperstumiami, reikšmė priskiriama į `data_[size_++]`.

Kai `size_ >= capacity_`, prieš įrašymą talpa padvigubinama (tuščiam konteineriui — `1`) kviečiant `reserve`. Kitu atveju naudojama jau rezervuota vieta buferyje.

```cpp
void CustomVector<T>::push_back(const T& value) {
    if (size_ >= capacity_) {
        const size_type new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
        reserve(new_cap);
    }
    data_[size_++] = value;
}
```

### `reserve(size_type new_cap)`

Didina `capacity_` nekeisdamas `size()`. Jei `new_cap <= capacity_` — nieko nedaro; jei viršija `max_size()` — `std::length_error`. Kitu atveju alokuoja naują masyvą ir perkelia elementus:

```cpp
void CustomVector<T>::reserve(size_type new_cap) {
    if (new_cap <= capacity_) {
        return;
    }
    if (new_cap > max_size()) {
        throw std::length_error("CustomVector::reserve");
    }

    auto new_data = std::make_unique<T[]>(new_cap);
    for (size_type i = 0; i < size_; ++i) {
        new_data[i] = std::move(data_[i]);
    }
    data_ = std::move(new_data);
    capacity_ = new_cap;
}
```

### `shrink_to_fit()`

Sumažina `capacity_` iki `size_`. Tuščiam konteineriui kviečia `reset()`:

```cpp
void CustomVector<T>::shrink_to_fit() {
    if (size_ == capacity_) {
        return;
    }
    if (size_ == 0) {
        reset();
        return;
    }

    auto new_data = std::make_unique<T[]>(size_);
    for (size_type i = 0; i < size_; ++i) {
        new_data[i] = std::move(data_[i]);
    }
    data_ = std::move(new_data);
    capacity_ = size_;
}
```

### `operator[](size_type pos)`

Grąžina `data_[pos]` be ribų tikrinimo. Palyginimui, `at()` meta `std::out_of_range`, jei `pos >= size_`:

`at()` su ribų tikrinimu:

```cpp
T& CustomVector<T>::at(size_type pos) {
    if (pos >= size_) {
        throw std::out_of_range("CustomVector::at");
    }
    return data_[pos];
}
```

`operator[]` be tikrinimo:

```cpp
T& CustomVector<T>::operator[](size_type pos) {
    return data_[pos];
}
```

### `insert(T* pos, const T& value)`

Įterpia kopiją prieš `pos`. Naudoja `begin()` / `end()` validacijai, `max_size()` ribai, `reserve` augimui, `std::move` perstūmimui; grąžina `data_.get() + index`:

```cpp
T* CustomVector<T>::insert(T* pos, const T& value) {
    if (pos < begin() || pos > end()) {
        throw std::out_of_range("CustomVector::insert");
    }
    if (size_ >= max_size()) {
        throw std::length_error("CustomVector::insert");
    }

    const size_type index = static_cast<size_type>(pos - begin());

    if (size_ >= capacity_) {
        const size_type new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
        reserve(new_cap);
    }

    for (size_type i = size_; i > index; --i) {
        data_[i] = std::move(data_[i - 1]);
    }
    data_[index] = value;
    ++size_;
    return data_.get() + index;
}
```

### `CustomVector` unit testai

Catch2 testai: `src/test/unit-test/custom-vector-catch2-test.cpp`.

```bash
make test
./test "[custom-vector]"
```

### `push_back` našumo palyginimas

Interaktyviame meniu pasirinkite `10 - Palyginti CustomVector ir std::vector push_back`, tada elementų skaičių (10 000 … 100 000 000). Testas vienoje sesijoje iš eilės matuoja `CustomVector<int>::push_back` ir `std::vector<int>::push_back`, kiekvienam dydžiui į galą įdedant `int` reikšmes `0 … count - 1`. Išvedamas kiekvienos iteracijos laikas ir **vidurkis** (sekundėmis).

Žemiau — vidurkiai po vienos iteracijos šioje mašinoje (žr. [Sistemos specifikacijas](#sistemos-specifikacijos)). `CustomVector` dažniausiai ~2× lėtesnis už `std::vector`, nes `reserve` per `make_unique<T[]>` konstruoja visą naują buferį, o ne tik naudojamus elementus.

| Elementų skaičius | `CustomVector<int>` (s) | `std::vector<int>` (s) |
| --- | ---: | ---: |
| 10 000 | 0.001414 | 0.000468 |
| 100 000 | 0.011501 | 0.005285 |
| 1 000 000 | 0.075264 | 0.036119 |
| 10 000 000 | 1.202800 | 0.428530 |
| 100 000 000 | 9.594113 | 4.196679 |

### Atminties perskirstymų skaičiavimas (V1.5)

Meniu punktas `11` užpildo konteinerį **100 000 000** `int` elementų ir skaičiuoja, kiek kartų prieš `push_back` buvo `size() == capacity()` (t. y. nebėra vietos naujam elementui — įvyks perskirstymas).

Skaičiavimas abiem konteineriams (`src/test/runtime-test/custom-vector-reallocation-test.cpp`):

```cpp
template <typename Vector>
std::size_t countReallocationsWhilePushBack(std::size_t element_count) {
    Vector vector;
    std::size_t reallocations = 0;

    for (std::size_t i = 0; i < element_count; ++i) {
        if (vector.size() == vector.capacity()) {
            ++reallocations;
        }
        vector.push_back(static_cast<typename Vector::value_type>(i));
    }

    return reallocations;
}
```

Abu konteineriai naudoja talpos padvigubinimą, todėl rezultatas turėtų **sutapti** (šioje mašinoje — **28** perskirstymai).

### Skirstymo strategijos: `CustomVector` vs `std::vector`

Meniu punktas `8` matuoja failo nuskaitymą, studentų rūšiavimą (pagal galutinį pažymį) ir skirstymą į `nuskriausti` / `protingi` pasirinkta strategija. Konteineris — `1` (`std::vector<Student>`) arba `4` (`CustomVector<Student>`); strategija — `1` (Pirma), `2` (Antra) arba `3` (Trečia).

Žemiau — bendro laiko (**Iš viso**) vidurkiai per 5 iteracijas (sekundėmis; žr. [Sistemos specifikacijas](#sistemos-specifikacijos)).

#### Strategija 1 — Pirma

| Failas | `CustomVector<Student>` (s) | `std::vector<Student>` (s) |
| --- | ---: | ---: |
| `students1000` | 0.002202 | 0.006700 |
| `students10000` | 0.021714 | 0.055347 |
| `students100000` | 0.243164 | 0.594252 |
| `students1000000` | 2.848959 | 6.451447 |
| `students10000000` | 34.240719 | 72.983393 |

#### Strategija 2 — Antra

| Failas | `CustomVector<Student>` (s) | `std::vector<Student>` (s) |
| --- | ---: | ---: |
| `students1000` | 0.379980 | 0.005223 |
| `students10000` | 0.036150 | 0.053774 |
| `students100000` | 0.317306 | 0.595528 |
| `students1000000` | 2.829029 | 6.951227 |
| `students10000000` | 32.586835 | 74.755801 |

#### Strategija 3 — Trečia

| Failas | `CustomVector<Student>` (s) | `std::vector<Student>` (s) |
| --- | ---: | ---: |
| `students1000` | 0.340612 | 0.006150 |
| `students10000` | 0.021888 | 0.063072 |
| `students100000` | 0.257629 | 0.597970 |
| `students1000000` | 2.959265 | 6.506724 |
| `students10000000` | 34.551087 | 78.380378 |

## Testų rezultatai: `struct` ir `class`

Žemiau pateikti testai, kuriuose buvo lyginama, kaip programa veikia naudojant `struct` ir `class` studentų aprašymui.

### `struct`

![Testų rezultatai su struct](assets/student-struct.png)

### `class`

![Testų rezultatai su class](assets/student-class.png)

Pastaba: rezultatai priklauso nuo aparatinės įrangos, kompiliatoriaus versijos, disko spartos ir tuo metu veikiančių foninių procesų. Šiuose testuose naudoti failai `students100000` ir `students1000000`, o kiekvienas matavimas buvo vidurkinamas per 5 iteracijas.

## Išvados

Iš šių bandymų matyti, kad `struct` versija šiek tiek greičiau atliko abu testuotus scenarijus nei `class` versija. Skirtumas buvo matomas tiek failo nuskaitymo, tiek rūšiavimo, tiek bendro laiko rezultatuose.

Didėjant įvesčiai, skirtumas tarp abiejų variantų išliko pastebimas, tačiau didžiausią laiką vis tiek sudarė duomenų nuskaitymas ir rūšiavimas.

## Papildomas testas: kompiliatoriaus optimizavimo lygiai

Papildomai buvo atliktas testas, kuriame lyginami tie patys scenarijai, tačiau keičiant kompiliatoriaus optimizavimo lygį (`-O1`, `-O2`, `-O3`).

### `struct` su skirtingais optimizavimo lygiais

#### `-O1`

![Testų rezultatai su struct -O1](assets/student-struct-o1.png)

#### `-O2`

![Testų rezultatai su struct -O2](assets/student-struct-o2.png)

#### `-O3`

![Testų rezultatai su struct -O3](assets/student-struct-o3.png)

### `class` su skirtingais optimizavimo lygiais

#### `-O1`

![Testų rezultatai su class -O1](assets/student-class-o1.png)

#### `-O2`

![Testų rezultatai su class -O2](assets/student-class-o2.png)

#### `-O3`

![Testų rezultatai su class -O3](assets/student-class-o3.png)

### Lentelė: vykdomojo failo dydis ir laikai

Žemiau suvesti tie patys rezultatai, kurie matomi aukščiau esančiose ekrano nuotraukose (vidurkiai per 5 iteracijas, laikas sekundėmis), kartu su vykdomojo `main` dydžiu kilobaitais. **KB** čia reiškia 1024 baitus (`wc -c main` ÷ 1024, vienas skaitmuo po kablelio). `main` dydžiai išmatuoti po `make clean` ir `make CXXFLAGS="-std=c++17 -Wall -Wextra -Iinclude -<lygis>"` su `g++` 13.3.0; esant identiškam `Student` išdėstymui, `struct` ir `class` raktiniai žodžiai davė tą patį dydį kiekvienam `-O` lygiui.

Stulpeliai **100k Viso** / **1M Viso** atitinka failų `students100000` ir `students1000000` bendrą laiką (sekundėmis).

| Realizacija | Lygis | `main` (KB) | 100k Viso (s) | 1M Viso (s) |
| --- | --- | ---: | ---: | ---: |
| `struct` | `-O1` | 253.9 | 0.370915 | 4.305099 |
| `struct` | `-O2` | 250.4 | 0.368166 | 4.306130 |
| `struct` | `-O3` | 287.8 | 0.372487 | 4.331548 |
| `class` | `-O1` | 254.7 | 0.403521 | 4.895525 |
| `class` | `-O2` | 251.1 | 0.441929 | 4.812805 |
| `class` | `-O3` | 288.6 | 0.390308 | 4.574972 |

Trumpa įžvalga: didinant optimizavimo lygį, bendras vykdymo laikas mažėja abiem realizacijoms, o didžiausias pagerėjimas dažniausiai matomas pereinant nuo `-O1` prie `-O2`.

## Struktūra

- `include/` - antraštiniai failai
- `src/` - pagrindinis programos kodas
- `assets/` - projekto vaizdai
- `data/` - pradiniai duomenų failai
- `students1000`, `students10000`, `students100000`, `students1000000`, `students10000000` - didelių imčių failai bandymams

## Pastabos

- Programa naudoja `make` failą surinkimui.
- Rikiavimas veikia tiek su `vector`, tiek su `list`, tiek su `deque`.
- Didelių duomenų imčių testai skirti palyginti nuskaitymo, rūšiavimo ir skaidymo laikus.
