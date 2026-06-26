import re
import os

# --- NASTAVENÍ ---
# Sem napiš přesný název tvého souboru (např. "data.json" nebo "studenti.txt")
NAZEV_SOUBORU = "trida.json" 
# -----------------

def smaz_weights_blok():
    if not os.path.exists(NAZEV_SOUBORU):
        print(f"Chyba: Soubor '{NAZEV_SOUBORU}' nebyl ve složce nalezen!")
        return

    # 1. Načtení celého souboru
    with open(NAZEV_SOUBORU, "r", encoding="utf-8") as f:
        obsah = f.read()

    # 2. Brutální regulární výraz, který ignoruje mezery a konce řádků
    # Hledá "weights": { ... cokoli uvnitř ... } a případnou čárku za tím
    vzor = r'"weights"\s*:\s*\{[\s\S]*?\}\s*,?'

    # 3. Spočítáme, kolikrát to v souboru vůbec je
    nalezeno = len(re.findall(vzor, obsah))
    
    if nalezeno == 0:
        print("Python taky nic nenašel. Zkontroluj, zda se ten klíč opravdu jmenuje \"weights\"!")
        return

    # 4. Nahrazení bloku prázdným řetězcem (smazání)
    novy_obsah = re.sub(vzor, "", obsah)

    # 5. Zápis upraveného textu zpět do souboru
    with open(NAZEV_SOUBORU, "w", encoding="utf-8") as f:
        f.write(novy_obsah)

    print(f"Úspěch! Blok 'weights' byl smazán na {nalezeno} místech.")

if __name__ == "__main__":
    smaz_weights_blok()