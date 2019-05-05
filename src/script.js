let deviceID = "3f0034000247363339343638"
let accessToken = "73b453915ae24e95491149695e12a85eca1c2ef2"

let variable = ["water", "temperature", "light"]
let results = []
  
for (let i = 0; i < 3; i++){
  let url = "https://api.particle.io/v1/devices/"+deviceID+"/"+variable[i]+"?access_token=" + accessToken
  let req = new Request(url)
  let json = await req.loadJSON()
  results[i] = json.result
}

let water = results[0]
let temprature = results[1]
let light = results[2]


let message;
if (water >= 40) {
  message = "fult med vann 🌊";
} else if (water >= 20) {
  message = "godt med vann 💦";
} else if (water >= 10) {
  message = "helt greit med vann igjen 💧";
} else if (water >= 5) {
  message = "litt lite vann igjen 🏝";
} else {
  message = "ikke noe er igjen 🏜";
}

let ligthMessage;
if (light >= 3700){
  ligthMessage = "Det er veldig mye sol ☀️";
} else if (light >= 3000) {
  ligthMessage = "Det er godt med sol🌤";
} else if (light >= 2000) {
  ligthMessage = "Det er ok med sol ⛅️";
} else if (light >= 1200) {
  ligthMessage = "Det er lite sol 🌥";
} else {
  ligthMessage = "Det er ikke noe sol ☁️";
}

QuickLook.present("Planta har " + message 
+ " Tempraturen i leiligheten din er " + temprature + "°C" 
+ ((temprature >= 20) ? " så varmt nok for planta. ":" så det er litt kjørlig. \n") 
+ ligthMessage)

Script.complete()