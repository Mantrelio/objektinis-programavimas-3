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

### Bendros vidinės / pagalbinės funkcijos

Šios funkcijos dažnai pasitaiko keliuose viešuosiuose metoduose:

| Funkcija | Paskirtis |
| --- | --- |
| `begin()` | Grąžina žymeklį į pirmą elementą: `data_.get()`. |
| `end()` | Grąžina žymeklį **už paskutinio** elemento: `data_.get() + size_`. Naudojama ciklams ir įterpimui į galą (`insert` / `emplace` su `end()`). |
| `data()` | Tas pats buferis kaip `begin()` — žaliųjų rodyklių prieiga prie masyvo. |
| `size()` / `capacity()` | Grąžina `size_` ir `capacity_` (tik skaitymas, `noexcept`). |
| `max_size()` | Didžiausias leistinas elementų skaičius: ribojamas `std::ptrdiff_t` ir `sizeof(T)`. Naudojama prieš augimą — jei viršyta, metama `std::length_error`. |
| `reset()` (private) | `data_.reset()`, `size_ = 0`, `capacity_ = 0` — visiškai ištuština buferį. |
| `reserve(new_cap)` | Perskirsto buferį į didesnį masyvą (žr. žemiau). |
| `std::move` | Perkelia esamus elementus į naują vietą be perteklinių kopijų (svarbu `reserve`, `insert`, `shrink_to_fit`). |
| `std::make_unique<T[]>(n)` | Alokuoja naują dinaminį masyvą `n` elementų. |
| Placement `new` | `emplace` sunaikina seną objektą ir **vietoje** sukuria naują: `new (data_.get() + index) T(...)`. |

### `push_back(const T& value)`

Prideda kopiją į konteinerio galą.

- Viešas metodas tik perduoda darbą: `emplace(end(), value)`.
- `size()` padidėja vienetu; esami elementai išlieka savo vietose.

```cpp
CustomVector<std::string> v;
v.push_back("a");
v.push_back("b");  // v == {"a", "b"}
```

**Viduje kviečiama:**

1. **`end()`** — nustato įterpimo vietą gale (indeksas lygus `size_`).
2. **`emplace(pos, value)`** — tikrasis darbas:
   - **`begin()`** ir **`end()`** — tikrinama, ar `pos` intervale `[begin(), end()]`; kitaip `std::out_of_range`.
   - **`max_size()`** — jei `size_ >= max_size()`, `std::length_error`.
   - Indeksas: `index = pos - begin()` (žymeklių aritmetika).
   - Jei `size_ >= capacity_`: **`reserve`** su `new_cap = (capacity_ == 0) ? 1 : capacity_ * 2`.
   - Ciklas `i` nuo `size_` žemyn iki `index`: `data_[i] = std::move(data_[i - 1])` — atlaisvina vietą.
   - **`data_[index].~T()`** ir placement **`new (data_.get() + index) T(value)`** — elementas sukuriamas vietoje.
   - `++size_`; grąžinama `data_.get() + index` (čia `push_back` grąžinamos reikšmės nenaudoja).

### `reserve(size_type new_cap)`

Iš anksto rezervuoja atmintį bent `new_cap` elementams **nekeisdamas** `size()`.

- Jei `new_cap <= capacity_`, metodas nieko nedaro.
- Jei `new_cap > max_size()`, metamas `std::length_error`.
- Priešingu atveju alokuojamas naujas masyvas, esami elementai perkeliami, atnaujinama tik `capacity_`.

Tai naudinga, kai iš anksto žinomas apytikslis elementų skaičius ir norima sumažinti perteklinius perskirstymus kviečiant `push_back` ar `insert`.

```cpp
CustomVector<int> v;
v.reserve(8);     // capacity >= 8, size() vis dar 0
v.push_back(1);
v.push_back(2);   // dažnai be papildomo perskirstymo
```

**Viduje naudojama:**

1. **`max_size()`** — viršutinė riba prieš alokaciją.
2. **`std::make_unique<T[]>(new_cap)`** — naujas buferis `new_data`.
3. Ciklas `for (i = 0; i < size_; ++i)`: **`new_data[i] = std::move(data_[i])`** — perkeliami tik jau egzistuojantys (`size_`) elementai; `size_` nesikeičia.
4. **`data_ = std::move(new_data)`** — senas masyvas automatiškai sunaikinamas per `unique_ptr`.
5. **`capacity_ = new_cap`** — atnaujinama tik talpa.

`reserve` **nekviečia** `reset()`, `shrink_to_fit()` ar `resize()` — tik didina talpą, jei reikia.

### `shrink_to_fit()`

Sumažina `capacity_` iki `size_` (atlaisvina nenaudojamą atmintį). Elementų turinys ir eilės tvarka nepasikeičia.

```cpp
CustomVector<int> v = {1, 2, 3};
v.reserve(16);
v.shrink_to_fit();  // capacity() == 3, turinys {1, 2, 3}
```

**Viduje naudojama:**

1. Palyginimas **`size_ == capacity_`** — ankstyvas išėjimas, jei pertekliaus nėra.
2. Jei **`size_ == 0`**: kviečiamas **`reset()`**:
   - **`data_.reset()`** — atlaisvina buferį;
   - **`size_ = 0`**, **`capacity_ = 0`**.
3. Kitu atveju:
   - **`std::make_unique<T[]>(size_)`** — masyvas tik reikiamo dydžio;
   - ciklas su **`std::move(data_[i])`** į `new_data[i]` (`i` nuo `0` iki `size_ - 1`);
   - **`data_ = std::move(new_data)`**, **`capacity_ = size_`**.

`shrink_to_fit` **nekviečia** `reserve()` — talpa tik mažinama, ne didinama.

### `operator[](size_type pos)`

Grąžina nuorodą į elementą pozicijoje `pos`: `return data_[pos]`.

- Skirtingai nuo `at(pos)`, **netikrina** ribų — neegzistuojantis indeksas yra neapibrėžtas elgesys (kaip ir `std::vector::operator[]`).
- Tinka, kai indeksas jau patikrintas arba garantuotai teisingas (ciklai `0 .. size()-1`).

```cpp
CustomVector<int> v = {10, 20, 30};
v[1] = 21;           // leidžiama, jei size() > 1
int x = v.at(2);     // saugiau: meta išimtį, jei pos >= size()
```

**Viduje naudojama:**

- Tiesiog **`data_[pos]`** per `std::unique_ptr<T[]>::operator[]` — jokio `begin()`, `size()` ar `capacity()` kvietimo.
- **`at(pos)`** (alternatyva): pirmiausia **`if (pos >= size_)`** → `std::out_of_range`, tada taip pat `return data_[pos]`.

`operator[]` yra paprasčiausias prieigos metodas; saugumą užtikrina tik kvietėjas arba `at()`.

### `insert(T* pos, const T& value)`

Įterpia **kopiją** `value` prieš poziciją `pos` ir grąžina žymeklį į naują elementą.

```cpp
CustomVector<int> v = {1, 3};
v.insert(v.begin(), 0);           // {0, 1, 3}
v.insert(v.begin() + 2, 2);       // {0, 1, 2, 3}
v.insert(v.end(), 4);             // {0, 1, 2, 3, 4}
```

**Pozicija ir ribos:**

- **`begin()`**, **`end()`** — `pos` turi būti `[begin(), end()]`; kitaip `std::out_of_range`.
- **`max_size()`** — jei `size_ >= max_size()`, `std::length_error`.

**Indeksas ir talpa:**

- **`index = static_cast<size_type>(pos - begin())`** — žymeklis paverčiamas skaitiniu indeksu.
- Jei `size_ >= capacity_`: **`reserve(new_cap)`**, kur `new_cap = (capacity_ == 0) ? 1 : capacity_ * 2` (tas pats augimo principas kaip `emplace`).

**Perstūmimas ir įrašymas:**

- Ciklas `i` nuo `size_` žemyn: **`data_[i] = std::move(data_[i - 1])`** — elementai nuo `index` stumiami į dešinę.
- **`data_[index] = value`** — kopijuojamas naujas elementas (ne placement `new`, skirtingai nuo `emplace`).
- **`++size_`**; grąžinama **`data_.get() + index`**.

**Susiję metodai (nekviečiami tiesiogiai iš `insert`, bet ta pati logika):**

- **`emplace(pos, args...)`** — vietoje kopijos naudoja `~T()` + placement `new` ir `std::forward<Args>`.
- **`insert_range(pos, first, last)`** — keliems elementams: **`std::distance`**, didesnis `reserve`, platesnis perstūmimo ciklas.
- **`push_back(value)`** — iš esmės `insert` į galą per **`emplace(end(), value)`**.

Papildomai klasėje yra `insert_range(pos, first, last)` ir `emplace(pos, args...)` (žr. aukščiau).

### `CustomVector` unit testai

Catch2 testai: `src/test/unit-test/custom-vector-catch2-test.cpp`.

```bash
make test
./test "[custom-vector]"
```

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
