Program pro učitele, který automaticky navrhuje zasedací pořádek žáků na základě zadaných kritérií pomocí optimalizačního algoritmu.

---

## Obsah

- [Jak instalovat](#jak-instalovat)
- [První spuštění](#první-spuštění)
- [Přidávání a prohazování lavic](#přidávání-a-prohazování-lavic)
- [Přidávání žáků](#přidávání-žáků)
- [Nastavení váhy faktorů](#nastavení-váhy-faktorů)
- [Automatické generování zasedacího pořádku](#automatické-generování-zasedacího-pořádku)
- [Export do PDF a správa souborů](#export-do-pdf-a-správa-souborů)
- [Hlášení problémů či námětů pro vylepšení](#Hlášení-problémů-či-námětů-pro-vylepšení)
- [Licence](#licence)

---

## Jak instalovat

> ⚠️ **Upozornění:** Program neobsahuje žádný malware jakéhokoli typu. Antiviry jej přesto velmi často označí jako škodlivý. Hláškám nemusíte věřit.

### Krok 1: Stažení

Vpravo na postranním panelu najděte sekci **„Releases"** a klikněte na **„Tags"**. Otevře se stránka s verzemi, klikněte na nejnovější a pak z assets stáhněte instalátor (`.exe` soubor).

V některých prohlížečích je zapotřebí dodatečně potvrdit, že si chcete soubor ponechat. Je to kvůli antivirové ochraně některých prohlížečů.

### Krok 2: Spuštění instalátoru

Z prohlížeče nebo průzkumníku souborů spusťte instalátor. Pokud se Vám ukáže modré okno s hláškou **„Windows ochránil Váš počítač"** nebo podobné, klikněte na **„Více informací"** a **„Přesto spustit"**.

Dále se otevře průvodce instalací, který Vám další postup vysvětlí.

---

## První spuštění

Po spuštění programu se zobrazí pracovní plocha, na které se vytváří a upravuje samotný zasedací pořádek.

**Pohyb v pracovní ploše:**

- **Posun (pan):** Stiskněte a podržte **pravé tlačítko myši** a tažením posouvejte celou plochu libovolným směrem.
- **Přiblížení / oddálení (zoom):** Použijte **kolečko myši** — scrollováním nahoru přiblížíte, scrollováním dolů oddálíte pohled na plochu.

---

## Přidávání a prohazování lavic

**Přidání lavice:** Místo pro žáka přidáte kliknutím na libovolné políčko na ploše. Tlačítkem **„+"** lze na dané místo manuálně vybrat, který žák tam bude sedět. Kliknutím na políčko s lavicí lavici smažete.

**Prohazování míst:** Kliknutím na tlačítko se symbolem obousměrné šipky (v horní liště) přepnete do režimu prohazování — tlačítko zmodrá. Poté klikněte na dvě pole, která chcete vzájemně prohodit. Prohodit lze libovolnou kombinaci:

- prázdné místo ↔ prázdné místo
- prázdné místo ↔ prázdná lavice
- lavice se žákem ↔ prázdné místo
- lavice se žákem ↔ prázdná lavice
- prázdná lavice ↔ prázdná lavice
- lavice se žákem ↔ lavice se žákem
---

## Přidávání žáků

Do nabídky správy žáků se dostanete přes menu **„Seznam žáků"**. Zde můžete přidávat nové žáky nebo upravovat jejich parametry v okně **„Upravit žáka"** (otevře se tlačítkem **„..."** u žáka v seznamu).

**Přehled parametrů žáka:**

| Parametr | Popis |
|---|---|
| **Jméno** | Křestní jméno a příjmení žáka. |
| **Výška (cm)** | Číselný údaj o výšce žáka. Vyšší žáci jsou algoritmem směřováni spíše do zadních řad, aby nebránili ve výhledu nižším spolužákům. |
| **Pohlaví** | Výběr ze seznamu (např. Muž, Žena apod.). Slouží pro pravidla kombinování pohlaví v lavicích. |
| **Může sedět s žákem jiného pohlaví** | Zaškrtávací pole. Pokud není zaškrtnuto, algoritmus bude žákovi vybírat sousedy pouze stejného pohlaví. Toto je **striktní pravidlo**, které algoritmus nesmí za žádných okolností porušit.|
| **Potřeba sedět vpředu** | Posuvník od 0.0 (nevyžaduje) do 1.0 (maximální priorita). Určeno pro žáky se zrakovým/sluchovým omezením nebo potřebou zvýšeného dohledu. |
| **Chce sedět s... (preferované)** | Seznam spolužáků seřazený dle priority (1., 2., 3. místo). Pořadí můžete měnit šipkami ▲ a ▼. Jde o preferenci, kterou se algoritmus snaží maximálně naplnit. Žáci neumístění v seznamu jsou považováni za neutrální. |
| **Nesmí sedět s... (striktní pravidlo)** | Seznam zakázaných sousedů. Toto je **striktní pravidlo**, které algoritmus nesmí za žádných okolností porušit. |

> **Poznámka:** Striktní omezení mají absolutní přednost před ostatními. Pokud nastavíte vzájemně si odporující striktní pravidla, algoritmus Vás upozorní.

---

## Nastavení váhy faktorů

V menu **„Váhy..."** můžete jemně doladit prioritizaci jednotlivých faktorů při výpočtu:

| Faktor | Popis |
|---|---|
| **Výška** | Určuje, jak přísně se dbá na to, aby vyšší žáci seděli za nižšími. |
| **Potřeba být vpředu** | Určuje důležitost předních řad pro žáky s vysokým požadavkem na sezení vpředu. |
| **Chce sedět s** | Váha přikládaná splnění přání sedět s konkrétním kamarádem. |
| **Je blízko u** | Bodovací bonus za sezení v těsné blízkosti (stejná lavice nebo sousední lavice). |
| **Je blízko u dosah** | Maximální vzdálenost (okruh), ve které algoritmus ještě považuje sousedy za „blízké". |

Tlačítko **„Výchozí"** v okně vah vrátí všechny posuvníky na tovární nastavení.

---

## Automatické generování zasedacího pořádku

Pokud nechcete žáky posazovat na místa ručně tlačítkem **„+"** u každé lavice, kliknutím na tlačítko **„Vygenerovat..."** v horní liště otevřete dialog pro volbu metody generování:

- **Náhoda** — Rozsadí žáky do dostupných lavic zcela náhodně bez ohledu na výšku, preference či omezení. Vhodné pro rychlé losování. Nebere ohled na zadané preference a striktní pravidla.
- **Hledací algoritmus** — Pokročilý optimalizační výpočet. Vyhodnocuje všechna zadaná kritéria, respektuje striktní pravidla a maximalizuje celkovou spokojenost třídy podle nastavených vah. (Algoritmus se nesnaží o nejlepší *průměrnou* spokojenost — cílí na to, aby byli spokojeni všichni alespoň nějak, a neupřednostňuje pár extrémně spokojených žáků na úkor jiných).

---

## Export do PDF a správa souborů

V nabídce **„Soubor"** naleznete standardní funkce pro správu projektu:

- **Nový / Otevřít / Uložit / Uložit jako** — Správa projektových souborů zasedacího pořádku (`.zsp`).
- **Exportovat do PDF...** — Vygeneruje čistý tiskový dokument. Před samotným exportem zadáte **Nadpis** (např. *Třída 8.A*) a **Podnadpis** (např. *Školní rok 2026/2027*, nebo jméno třídního učitele), které se zobrazí v záhlaví výsledného PDF plánku.

---
## Hlášení problémů či námětů pro vylepšení
Všechny problémy a náměty na vylepšení prosím hlaste přes sekci **Issues** na GitHubu:

1. Přejděte do repozitáře projektu na GitHubu.
2. V horní nabídce klikněte na záložku **Issues**.
3. Klikněte na zelené tlačítko **New issue**.
4. Zadejte výstižný název a do popisu detailně uveďte:
   * Popis chování (co se stalo a co jste očekávali).
   * Postup, jak chybu vyvolat (kroky k reprodukci).
   * Případně přiložte chybovou hlášku.
---
## Licence

Program je zdarma pro nekomerční použití. Kompletní licenční ujednání (EULA) včetně licencí použitých knihoven třetích stran (SDL2, nlohmann/json, pdfgen, tinyfiledialogs, maximum-weight-matching) se zobrazí během instalace, nebo jej najdete v souboru `license_agreement.txt`.
