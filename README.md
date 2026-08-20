Program pro učitele, který automaticky navrhuje zasedací pořádek žáků na základě zadaných kritérií pomocí optimalizačního algoritmu.

---

## Obsah

- [Jak instalovat](#jak-instalovat)
- [První spuštění](#první-spuštění)
- [Přidávání žáků](#přidávání-žáků)
- [Nastavení váhy faktorů](#nastavení-váhy-faktorů)
- [Spuštění výpočtu](#spuštění-výpočtu)
- [Export do PDF a správa souborů](#export-do-pdf-a-správa-souborů)
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

> 🚧 *TODO: Doplnit popis hlavního okna programu po prvním spuštění — co uživatel vidí, jaké panely/nabídky jsou k dispozici, jak vypadá pracovní plocha se zasedacím pořádkem.*

---

## Přidávání žáků

> 🚧 *TODO: Doplnit postup, jak se do programu zadávají žáci — jméno, třída, případně další atributy (výška, přání sedět s kamarádem, potřeba sedět vpředu), a jak se tato data následně upravují nebo mažou.*

---

## Nastavení váhy faktorů

V menu **„Váhy..."** můžete jemně doladit prioritizaci jednotlivých faktorů při výpočtu:

| Faktor | Popis |
|---|---|
| **Výška** | Určuje, jak přísně se dbá na to, aby vyšší žáci seděli za nižšími. |
| **Potřeba být vpředu** | Určuje důležitost předních řad pro žáky s vysokým požadavkem na sezení vpředu. |
| **Chce sedět s** | Váha přikládaná splnění přání sedět s konkrétním kamarádem. |
| **Je blízko u** | Bodovací bonus za sezení v těsné blízkosti (stejná lavice nebo sousední lavice). |
| **Je blízko v dosahu** | Maximální vzdálenost (okruh), ve které algoritmus ještě považuje sousedy za „blízké". |

Tlačítko **„Výchozí"** v okně vah vrátí všechny posuvníky na tovární nastavení.

---

## Spuštění výpočtu

> 🚧 *TODO: Doplnit popis, jak se spouští samotný výpočet zasedacího pořádku (které tlačítko, jak dlouho to trvá, jak se zobrazí/potvrdí výsledek, případně jak lze výsledek ručně upravit).*

---

## Export do PDF a správa souborů

V nabídce **„Soubor"** naleznete standardní funkce pro správu projektu:

- **Nový / Otevřít / Uložit / Uložit jako** — Správa projektových souborů zasedacího pořádku (`.json` / `.dat`).
- **Exportovat do PDF...** — Vygeneruje čistý tiskový dokument. Před samotným exportem zadáte **Nadpis** (např. *třída 8.A*) a **Podnadpis** (např. *Školní rok 2026/2027*), které se zobrazí v záhlaví výsledného PDF plánku.

---

## Licence

Program je zdarma pro nekomerční použití. Kompletní licenční ujednání (EULA) včetně licencí použitých knihoven třetích stran (SDL2, nlohmann/json, pdfgen, tinyfiledialogs, maximum-weight-matching) se zobrazí během instalace, nebo jej najdete v souboru `license_agreement.txt`.
