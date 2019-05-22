# Dokumentasjon
# Idé
I denne oppgaven har jeg valgt å lage en “dings” med hovedoppgave å passe på en plante i leiligheten min. Dette innebærer bruk av en rekke forskjellige sensorer.

Hvis planta mangler vann, det er for kaldt eller den har for lite lys, så blir bruker varslet. Man kan alltid se planta sin status enten ved å sjekke LCD skjermen festet til “dingsen” eller ved å si til Siri “Plante status”. Bruker vil også bli varslet gjennom en LED-pære festet til “dingsen”. 

Som en bonusfunksjon vil “dingsen” også passe på at leiligheten ikke brenner, da dette vil være katastrofalt for planta. Hvis flammesensoren observerer brann vil “dingsen” spille en liten trudelutt, LED-pæra blinke samt oppdatere en funksjon som vil gi videre ringvirkninger. Mer om dette i avsnittet _sky-tjenester_. 

## Video
Video av min oppkoblingen ligger i folderen _media_. Jeg snakker i videoen noe som kan av anonymisere meg. Hvis dette er et problem så anbefaler jeg bare å se videoen uten lyd. Videoen beskriver mye av det samme som står i dette dokumentet i tillegg til at jeg viser at den fungerer med Google Assistent.

Folderen inneholder i tillegg bilder av “dingsen”, oppkoblingen (bilde og _Fritzing_ skjema), hvilke _applets_ jeg har laget med IFTTT i tillegg til et par korte videoer som viser sky-funksjonalitet i bruk.

## Oppkobling
(img)
I oppgaven benyttes en rekke forskjellige sensorer. Under har jeg laget en liste over alle komponentene benyttet i oppgaven. Det er også et bilde i teksten av et Fritzing-skjema for å vise oppsettet jeg har laget. Skjemaet har riktignok noen feil da jeg ikke klarte å finne rette komponenter i Fritzing. Disse feilene er dokumentert i avsnittet _Feil i Frizing-skjemaet_. 

### Komponenter brukt
- Particle Proton
- Vannmålersensor
- LED-Lyspære
- Flammesensor
- LCD skjerm (16x2)
- Lyssensor
- Høyttaler
- 2x 10K resistorer
- 2x 220 resistorer 
- 4,7K resistor
- Temperaturmåler (DS18S20)

### Feil i Fritzing-skjemaet
Da jeg satte opp dette skjemaet fant jeg ikke en rekke av de sensoren jeg faktisk benyttet. I tillegg var det et stort problem å sette dem opp slik som det faktisk er satt opp i virkeligheten. Det er derfor mye mer spredd på tegningen enn hva det er i det virkelige oppsettet. 

#### Forskjeller
1. Motstand skjerm i virkeligheten 10K og ikke 220 som på tegningen.
2. Temperatursensor i virkeligheten DS18S20 og ikke DS18B20 som på tegningen. 
3. Motstand til DS18S20 er 4,7K og ikke 220 som på tegningen
4. Jeg fant ikke flammesensor i lista. Derfor har jeg satt inn en stor LED-pæra i stedet. 
5. Kablene til skjermen er ikke festet, da vi har en driver loddet fast på baksiden. Jeg valgte å sette inn skjermen og la kablene markere koblingen til driveren.
6. Vannmåler var ikke å finne i lista over sensorer. Derfor har jeg satt inn en jordfuktmåler, da den hadde samme oppsettet i toppen slik som vannsensoren. 

(img)
## Bruk av skytjeneste
Ved bruk av tjenesten IFTTT har jeg forlenget Particle Protonet sin funksjonalitet. Jeg har koblet deres API opp mot fire andre tjenester. I tillegg til dette skrev jeg et script som kjører på telefonen min. Dette gir Particle Proton tilgang til en ny funksjon som kom med iOS 12, nemlig Siri Shortcuts.

### IFTTT-varslinger
Jeg ønsket å sende meg selv varslinger når vann nivået til planta endret seg. Min første løsning på dette var å sende meg selv SMS-er. Dette viste seg å være problematisk (ved utvikling) da IFTTT kun sender 10 meldinger per måned. For å kunne se statusendringen tillot jeg at  IFTTT-applikasjonen kunne varsle meg. Varslinger er nå ubegrenset. 

### Påminnelser
Av og til er ikke et varsel nok og jeg vil kanskje trenge en påminnelse når jeg er hjemme. Hvis vannivået er mindre enn 5mm vil det automatisk bli generert en Påminnelse i “påminnelsesappen” på telefonen min. Den vil varsle meg kl19:00.

### Philips Hue 
I tillegg til å bruke varslinger benyttet jeg meg av Philips Hue. I leiligheten min bruker alle lamper Hue-lyspærer. Ved å lytte til Protonens variabler lagde jeg de to følgende kallene:  
1. Alle lys skrus på dersom det brenner i leiligheten
2. Dersom vannivået er under 5mm vil en av lampene i leiligheten blinke. Dette er demonstrert i en videoen _ blink light_ i _media_ folderen. 

### Google assistent
Den siste forlengelsen jeg bruker er Google Assistent. Dette er forlengelsen jeg ble mest skuffet over. Siden tjenesten heter _If This Then That_ er det innforstått at en ting kun kan føre til en annen. Det dumme med dette oppsettet er at det kun kan be Particle om å kjøre en funksjon. Ønsket mitt var å spørre Google Assistent om informasjon, som den hentet fra Protonen for så å lese dette opp.
(img)

### Siri Shortcut “Plante Status”
Jeg skrev et script som tillater Siri å oppdatere meg om den nåværende tilstanden til planta. Scriptet viser den samme informasjonen som det LCD-skjermen gjør. Fordelen her er at jeg nå kan sjekke denne informasjonen når jeg ikke er hjemme.

Dette scriptet er vedlagt. Hvis dette er ønskelig å teste, anbefaler jeg å laste ned applikasjonen Scriptable på en iOS-enhet. Er data fra min Proton ønsket så ligger `access token` og `Device ID` i toppen av dokumentet. Disse to kan enkelt byttes ut, dersom dere ønsker å replisere på en annen enhet.

## Nytteverdi
Denne gjenstanden har en reel nytteverdi for meg, da jeg allerede har klart å drepe et par planter ved å glemme å vanne dem. Jeg itererte over oppgaven flere ganger og la til mer og mer ettersom jeg kunne se bruksområder for tilleggs sensorer. Dette har blitt et prosjekt jeg verdsetter mye, og som jeg har ekspansjonsplaner for. Ved å ta dette emnet har jeg lært en rekke fascinerende ting. Det er en hel del hverdagsproblemer som enkelt kan bli løst med kun enkle _embedded_-kreasjoner.

Det endelige prosjektet føles som et sammenhengende produkt, hvor hver enkelt komponent spiller en rolle og at de sensorene som er benyttet kommer til sin rett. 
