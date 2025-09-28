/*
 * AI Choose Your Own Adventure Game
 * Using chinScreen library + LM Studio Local API
 */

// Enable required chinScreen features
#define CHINSCREEN_ENABLE_SHAPES
#define CHINSCREEN_ENABLE_ICONS
#define CHINSCREEN_ENABLE_ANIMATIONS
#define CHINSCREEN_ENABLE_CYOA_LARGE

// Enable specific icons
#define CHINSCREEN_ICON_USER
#define CHINSCREEN_ICON_SETTINGS
#define CHINSCREEN_ICON_REFRESH
#define CHINSCREEN_ICON_CYOA
#define LVGL_PORT_ROTATION_DEGREE 0
#include <chinScreen.h>

// WiFi and HTTP libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Configuration - Update these with your details
const char* WIFI_SSID = " ";
const char* WIFI_PASSWORD = " ";

// LM Studio Configuration
const char* LM_STUDIO_HOST = "192.168.1.1";  // Replace with your LM Studio host IP
const int LM_STUDIO_PORT = 1234;                // Default LM Studio port
const char* LM_STUDIO_MODEL = "local-model";    // This can be any string for local models

// Construct the full URL
String LM_STUDIO_URL = "http://" + String(LM_STUDIO_HOST) + ":" + String(LM_STUDIO_PORT) + "/v1/chat/completions";

// Game state
enum GameState {
  STATE_LOADING,
  STATE_GENRE_SELECT,
  STATE_TOPIC_SELECT,
  STATE_STORY,
  STATE_OPTIONS,
  STATE_WAITING,
  STATE_GAMEOVER,
  STATE_ERROR,
  STATE_SELECTION_TRANSITION
};

struct GamePage {
  int page_id;
  String text;
  String option_a_text;
  String option_b_text;
  String option_c_text;
  int option_a_next;
  int option_b_next;
  int option_c_next;
};

// Expanded Genre and Topic arrays
String genres[] = {
"Horror","Sci-Fi","Fantasy","Mystery","Thriller","Adventure","Romance","Western","Post-Apocalyptic","Superhero",
"Comedy","Cyberpunk","Steampunk","Dramedy","Satire","Parody","Dark Comedy","Rom-Com","Musical","Epic Fantasy",
"Urban Fantasy","Historical Fiction","Spy","Zombie","Vampire","Pirates","Medieval","Noir","Martial Arts","Alternate History",
"Real History","Biopic","Psychological Horror","Cosmic Horror","Slasher","Survival Horror","Gothic","Mythic","Epic","Fable",
"Fairy Tale","Folklore","Sword and Sorcery","Space Opera","Hard Sci-Fi","Soft Sci-Fi","Time Travel","Alternate Universe","Metafiction","Magical Realism",
"Speculative Fiction","Weird Fiction","Heroic Fantasy","High Fantasy","Low Fantasy","Techno-Thriller","Political Thriller","Legal Thriller","Medical Drama","Disaster",
"Creature Feature","Kaiju","Alien Invasion","First Contact","Space Western","Science Fantasy","Military Sci-Fi","Utopian","Dystopian","Climate Fiction",
"Solarpunk","Biopunk","Nanopunk","Clockpunk","Dieselpunk","Atompunk","Stonepunk","Mythpunk","Splatterpunk","Supernatural Thriller",
"Giallo","Haunted House","Monster Romance","Paranormal Romance","Dark Fantasy","Romantic Fantasy","Historical Romance","Regency Romance","Gothic Romance","Erotic Thriller",
"Teen Drama","Coming of Age","Young Adult Fantasy","Children’s Adventure","Middle Grade Fantasy","LitRPG","Isekai","Portal Fantasy","Metaphysical","Transgressive",
"Existentialist","Absurdist","Experimental","Avant-Garde","Minimalist","Surrealist","Impressionist","Expressionist","Dreamlike","Pulp",
"Camp","B-Movie","Grindhouse","Exploitation","Action","Disaster Action","Buddy Cop","Spy Action","Kung Fu","Samurai",
"Historical Epic","Mythological Epic","War Epic","Naval Adventure","Sea Voyage","Exploration","Colonial Adventure","Jungle Adventure","Treasure Hunt","Survival",
"Apocalyptic Survival","Post-Survival","Road Trip","Travelogue","Sports Drama","Sports Comedy","Sports Documentary","Martial Arts Fantasy","Supernatural Mystery","Occult Detective",
"True Crime","Crime Drama","Crime Procedural","Heist","Conspiracy Thriller","Detective Noir","Hardboiled","Police Procedural","Courtroom Drama","Prison Drama",
"Political Drama","Political Satire","Propaganda","Anti-War","War Drama","Trench Horror","Military Comedy","Military Romance","Medical Comedy","Medical Horror",
"Medical Thriller","Psychological Drama","Domestic Drama","Family Saga","Generational Saga","Historical Saga","Dynastic Epic","Futuristic Epic","Interplanetary Epic","Multiverse Epic",
"Philosophical Fiction","Didactic Fiction","Allegory","Moral Tale","Cautionary Tale","Revenge Tragedy","Courtly Love","Epic Poem","Verse Novel","Hauntology",
"Postmodern","Metamodern","Hypermodern","Retro-Futurism","Lost World","Hollow Earth","Atlantis Mythos","Eldritch Horror","Weird Western","Mythic Western",
"Gothic Western","Sci-Fi Western","Neo-Noir","Tech Noir","Folk Horror","Wilderness Horror","Eco-Horror","Body Horror","Paranormal Horror","Occult Horror",
"Found Footage","Mockumentary","Documentary","Biographical Drama","Historical Documentary","Music Documentary","Rockumentary","Concert Film","Animated Fantasy","Animated Sci-Fi",
"Animated Comedy","Stop Motion","Claymation","Anime","Manga Adaptation","Graphic Novel Adaptation","Comic Book Adaptation","Light Novel Adaptation","GameLit","Game Adaptation",
"Virtual Reality","Augmented Reality","Simulation Fiction","AI Fiction","Robot Fiction","Cyborg Fiction","Cloning Fiction","Genetic Engineering","Mutation Fiction","Evolutionary Fiction",
"Post-Human","Anthropocene Fiction","Alien Romance","Monster Comedy","Monster Buddy","Monster Hunter","Supernatural Comedy","Ghost Story","Haunting Comedy","Haunting Tragedy",
"Spiritual Journey","Visionary Fiction","Religious Fiction","Biblical Epic","Myth Retelling","Legend Retelling","Gods and Mortals","Creation Myth","End Times","Apocalypse",
"Post-Apocalypse","Prehistoric Fiction","Stone Age Fantasy","Bronze Age Epic","Iron Age Saga","Ancient Rome Epic","Ancient Greek Epic","Egyptian Mythos","Mesopotamian Saga","Celtic Fantasy",
"Norse Mythos","Aztec Mythos","Mayan Mythos","Incan Mythos","African Mythos","Chinese Mythos","Japanese Mythos","Indian Epic","Middle Eastern Epic","Native American Mythos",
"Aboriginal Dreamtime","Shamanic Journey","Vision Quest","Mystic Fantasy","Occult Fantasy","Demonology","Angelology","Heaven and Hell","Underworld Mythos","Afterlife Fiction",
"Ghost Romance","Poltergeist Horror","Possession Horror","Exorcism Horror","Witchcraft Horror","Witch Romance","Witch Comedy","Wizard School","Sorcery Saga","Alchemy Fiction",
"Rune Magic","Blood Magic","Forbidden Magic","Elemental Magic","Nature Magic","Celestial Magic","Void Magic","Shadow Magic","Light Magic","Chaos Magic",
"Order vs Chaos","Good vs Evil","Anti-Hero","Villain Protagonist","Monster Protagonist","Tragic Hero","Reluctant Hero","Chosen One","False Prophet","Messiah Fiction",
"AI Uprising","Robot Rebellion","Corporate Dystopia","Cybernetic Future","Space Colonization","Space Mining","Space Warfare","Terraforming","Galactic Empire","Interstellar Politics",
"Alien Diplomacy","Alien War","Alien Horror","Alien Comedy","Alien Buddy","Alien Family","Alien Infiltration","Alien Symbiosis","Alien Technology","Alien Religion",
"Time Loop","Alternate Timelines","Future War","Future Romance","Future Horror","Future Mystery","Quantum Fiction","Multiverse Fiction","Parallel Universes","String Theory Fiction",
"Simulation Theory","Cosmic Comedy","Cosmic Drama","Cosmic Horror","Cosmic Romance","Astrobiology Fiction","Exoplanet Adventure","Deep Space Horror","Astronaut Drama","Asteroid Disaster",
"Comet Disaster","Black Hole Fiction","Supernova Disaster","Solar Flare Disaster","Climate Disaster","Pandemic Horror","Pandemic Drama","Pandemic Comedy","Medical Apocalypse","Zombie Apocalypse",
"Robot Apocalypse","Alien Apocalypse","AI Apocalypse","Nuclear Apocalypse","Environmental Apocalypse","Magical Apocalypse","Biblical Apocalypse","Mythic Apocalypse","Dream Apocalypse","Endless War",
"Peace Utopia","Post-Utopia","Broken Utopia","Techno-Utopia","Techno-Dystopia","Bio-Dystopia","Nano-Dystopia","Corporate Utopia","Religious Utopia","Virtual Utopia",
"Virtual Dystopia","Digital Afterlife","Uploaded Consciousness","Mind Swap","Mind Upload","Dream Travel","Lucid Dreaming","Nightmare Fiction","Sleep Paralysis Horror","Hypnosis Fiction",
"Psychic Fiction","Telepathy Fiction","Telekinesis Fiction","Clairvoyance Fiction","Prophecy Fiction","Foresight Fiction","Fate vs Free Will","Destiny Drama","Chosen Destiny","Cursed Destiny",
"Cursed Love","Forbidden Romance","Secret Lovers","Enemies to Lovers","Friends to Lovers","Rivals to Lovers","Fake Dating","Marriage of Convenience","Love Triangle","Unrequited Love",
"Slow Burn Romance","Fast Romance","Instant Chemistry","Tragic Romance","Epic Romance","Star-Crossed Lovers","Second Chance Romance","Forbidden Desire","Dark Romance","Monster Romance",
"Alien Romance","AI Romance","Virtual Romance","Digital Love","Cyborg Love","Ghost Love","Vampire Love","Werewolf Love","Witch Love","Zombie Love"
};

String topics[] = {
"Dark","Crime","Conspiracy","Murder","Heist","Survival","War","Exploration","Betrayal","Revenge",
"Escape","Discovery","Invasion","Time Travel","Aliens","Magic","Creepy","Rebellion","Ancient Secrets","Lost Treasure",
"Forbidden Love","Political Intrigue","Supernatural","Undercover","Racing Against Time","Prophecy","Cursed Artifact","Hidden Identity","Memory Loss","Parallel Worlds",
"Redemption","Sacrifice","Corruption","Power Struggle","Forbidden Knowledge","Rise and Fall","Coming of Age","Destiny","Fate vs Free Will","Secret Societies",
"Hidden Agendas","Family Legacy","Ancient Prophecy","Cosmic Horror","Divine Intervention","Survival of the Fittest","Apocalypse","Resurrection","Immortality","Transformation",
"Dual Identity","Mind Control","Dreams vs Reality","Nightmares","Haunting","Possession","Madness","Obsession","Forbidden Science","Lost Civilization",
"Cloning","Genetic Engineering","Artificial Intelligence","Robot Rebellion","Machine Uprising","Time Loop","Alternate Timeline","Virtual Reality","Simulation","Mind Upload",
"Psychic Powers","Telepathy","Telekinesis","Precognition","Shape-shifting","Alchemy","Elemental Forces","Chaos vs Order","Light vs Darkness","Eternal War",
"Peace vs Conflict","Secrets of the Ancients","Mystic Journeys","Vision Quests","Shamanism","Occult Rituals","Blood Magic","Forbidden Rituals","Dark Bargains","Demonic Pacts",
"Angel vs Demon","Good vs Evil","Hero’s Journey","Villain’s Journey","Tragic Hero","Anti-Hero","Chosen One","Reluctant Hero","False Savior","Messiah Complex",
"Conspiracies","Government Cover-up","Corporate Control","Big Brother","Surveillance State","Resistance","Rebellion Rising","Underground Movement","Secret Weapons","Alien Technology",
"Lost Technology","Ancient Relics","Sacred Objects","Holy War","Religious Fanaticism","Prophetic Dreams","Signs and Omens","Apocalyptic Visions","End Times","Judgment Day",
"Natural Disasters","Climate Change","Floods","Earthquakes","Volcanoes","Tsunamis","Meteor Strikes","Pandemics","Famine","Plagues",
"Genocide","Extinction","Rebirth","Rebuilding Civilization","New World Order","Colonization","Terraforming","Galactic Empires","Space Warfare","First Contact",
"Alien Invasion","Peaceful Aliens","Hostile Aliens","Alien Symbiosis","Alien Experiments","Abductions","Ancient Aliens","Star Gates","Wormholes","Black Holes",
"Quantum Physics","String Theory","Parallel Dimensions","Alternate Universes","Multiverse","Cosmic Balance","Entropy","Chaos Theory","Butterfly Effect","Unintended Consequences",
"Forbidden Zones","Dangerous Journeys","Expeditions","Treasure Hunts","Hidden Valleys","Lost Cities","Atlantis","El Dorado","Shangri-La","Mystic Mountains",
"Frozen Wastes","Desert Wastelands","Jungles","Caverns","Oceans","Islands","Floating Cities","Sky Realms","Underworld","Afterlife",
"Ghosts","Spirits","Demons","Angels","Vampires","Werewolves","Zombies","Witches","Sorcerers","Shapeshifters",
"Monsters","Dragons","Giants","Titans","Mythical Beasts","Cyborgs","Androids","Clones","Mutants","Superhumans",
"Genetic Experiments","Lab Accidents","Forbidden Labs","Military Experiments","Weapons of Mass Destruction","Bioweapons","Nanotech","Viruses","Plagues","Contagion",
"Outbreaks","Quarantines","Isolation","Bunkers","Safe Havens","Last Survivors","Scavengers","Nomads","Travelers","Explorers",
"Inventors","Mad Scientists","Alchemists","Mystics","Prophets","Priests","Cult Leaders","Dictators","Kings and Queens","Dynasties",
"Empires","Revolutions","Uprisings","Cults","Secret Orders","Brotherhoods","Guilds","Clans","Tribes","Villages",
"Cities","Mega-Cities","Arcologies","Floating Nations","Space Stations","Colonies","Frontiers","Wild West","Urban Jungles","Rural Towns",
"Haunted Houses","Castles","Mansions","Catacombs","Graveyards","Crypts","Ruins","Temples","Shrines","Monasteries",
"Libraries","Labs","Universities","Schools","Witch Academies","Guild Halls","Training Grounds","Battlefields","Arenas","Gladiator Pits",
"Duels","Tournaments","Competitions","Races","Chases","Pursuits","Escapes","Captivity","Imprisonment","Exile",
"Punishment","Retribution","Rebellion","Revolt","Civil War","World War","Cold War","Proxy War","Cyber War","Information War",
"Propaganda","Mind Games","Psychological Warfare","Interrogations","Torture","Brainwashing","Manipulation","Deception","Betrayals","Lies",
"Secrets","Hidden Truths","Forbidden Desires","Temptation","Seduction","Passion","Unrequited Love","Star-Crossed Lovers","Love Triangles","Heartbreak",
"Second Chances","Lost Loves","Eternal Love","Unlikely Romance","Forbidden Romance","Dark Romance","Monster Romance","Alien Romance","AI Romance","Digital Love",
"Family Bonds","Parent and Child","Siblings","Generations","Dynasties","Lineage","Inheritance","Adoption","Orphans","Found Families",
"Friendship","Rivalry","Mentorship","Apprenticeship","Partnership","Betrayal of Trust","Loyalty","Sacrifice for Friends","Enemies to Friends","Friends to Lovers",
"Isolation","Loneliness","Despair","Hope","Faith","Belief","Doubt","Fear","Courage","Resilience",
"Transformation","Metamorphosis","Evolution","Devolution","Mutation","Ascension","Transcendence","Enlightenment","Illumination","Awakening",
"Madness","Insanity","Paranoia","Delirium","Hallucinations","Night Terrors","Psychosis","Obsessions","Addictions","Compulsions",
"Greed","Gluttony","Wrath","Envy","Lust","Pride","Sloth","Virtues","Justice","Mercy",
"Law","Order","Chaos","Anarchy","Balance","Disharmony","Harmony","Symbiosis","Parasites","Predators",
"Prey","Hunting","Gathering","Agriculture","Industry","Technology","Progress","Stagnation","Regression","Collapse",
"Rebuilding","Renewal","Cycles","Seasons","Ages","Eras","Epochs","Timelines","Histories","Legends",
"Myths","Fables","Fairy Tales","Stories","Songs","Poetry","Epic Sagas","Chronicles","Records","Journals",
"Diaries","Letters","Messages","Codes","Ciphers","Riddles","Puzzles","Clues","Mazes","Labyrinths",
"Trials","Tests","Initiations","Rites of Passage","Ceremonies","Coronations","Funerals","Weddings","Feasts","Festivals",
"Traditions","Customs","Cultures","Languages","Symbols","Runes","Glyphs","Sigils","Insignias","Banners",
"Flags","Heraldry","Icons","Relics","Artifacts","Totems","Idols","Talismans","Amulets","Charms",
"Blessings","Curses","Hexes","Spells","Incantations","Enchantments","Illusions","Summonings","Bindings","Banishing",
"Portents","Omens","Signs","Prophecies","Visions","Dreams","Nightmares","Forebodings","Warnings","Revelations",
"Discoveries","Inventions","Creations","Constructions","Destructions","Catastrophes","Miracles","Wonders","Marvels","Anomalies"
};



GameState currentState = STATE_LOADING;
GamePage currentPage;
String conversation_history = "";
String selectedGenre = "";
String selectedTopic = "";
String genreOptions[5];
String topicOptions[5];

// UI Objects
lv_obj_t* storyText = nullptr;
lv_obj_t* optionAButton = nullptr;
lv_obj_t* optionBButton = nullptr;
lv_obj_t* optionCButton = nullptr;
lv_obj_t* optionDButton = nullptr;
lv_obj_t* optionEButton = nullptr;
lv_obj_t* statusLabel = nullptr;
lv_obj_t* loadingIcon = nullptr;
lv_obj_t* gameOverLabel = nullptr;
lv_obj_t* loadingAnimation = nullptr;

// Function declarations
void setupWiFi();
void setupGame();
void showStoryPage();
void showOptionsPage();
void showLoadingPage();
void showGenreSelection();
void showTopicSelection();
void showGameOverPage();
void showErrorPage();
void showSelectionTransition(String selectedOption, String nextPageText);
bool requestNewStory(String userChoice = "");
void parseGameResponse(String jsonResponse);
void optionA_callback(lv_event_t* e);
void optionB_callback(lv_event_t* e);
void optionC_callback(lv_event_t* e);
void optionD_callback(lv_event_t* e);
void optionE_callback(lv_event_t* e);
void restart_callback(lv_event_t* e);
void generateRandomOptions();
bool isGameOver();
bool testLMStudioConnection();
String fixJsonQuotes(String jsonString);
void parseGameResponseFallback(String response);

void setup() {
  Serial.begin(115200);
  Serial.println("=== AI Choose Your Own Adventure Game (LM Studio) ===");
  
  // Initialize display
  init_display();
  setupGame();
  
  // Start with animated background
  chinScreen_background_solid("black");
  
  showLoadingPage();
  
  // Initialize WiFi
  setupWiFi();
  
  // Test LM Studio connection and start game
  if (WiFi.status() == WL_CONNECTED) {
    if (testLMStudioConnection()) {
      delay(2000); // Give time to see the loading screen
      showGenreSelection();
    } else {
      showErrorPage();
    }
  } else {
    showErrorPage();
  }
  
  Serial.println("=== Game Ready ===");
}

void loop() {
  chinScreen_task_handler();
  delay(5);
}

void setupWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    Serial.printf("WiFi attempt %d\n", attempts);
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected!");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi connection failed!");
  }
}

bool testLMStudioConnection() {
  Serial.println("Testing LM Studio connection...");
  
  HTTPClient http;
  http.begin(LM_STUDIO_URL);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(10000); // 10 second timeout
  
  // Simple test request to check if LM Studio is running
  DynamicJsonDocument testDoc(512);
  testDoc["model"] = LM_STUDIO_MODEL;
  
  JsonArray messages = testDoc.createNestedArray("messages");
  JsonObject userMsg = messages.createNestedObject();
  userMsg["role"] = "user";
  userMsg["content"] = "Hello";
  
  testDoc["max_tokens"] = 10;
  testDoc["temperature"] = 0.7;
  
  String jsonString;
  serializeJson(testDoc, jsonString);
  
  int httpResponseCode = http.POST(jsonString);
  
  if (httpResponseCode == 200) {
    Serial.println("LM Studio connection successful!");
    http.end();
    return true;
  } else {
    Serial.printf("LM Studio connection failed. HTTP Error: %d\n", httpResponseCode);
    Serial.println("Make sure LM Studio is running and the server is started.");
    Serial.printf("Expected URL: %s\n", LM_STUDIO_URL.c_str());
    http.end();
    return false;
  }
}

void setupGame() {
  chinScreen_clear();
  
  // Initialize game state
  currentPage.page_id = 0;
  conversation_history = "";
  selectedGenre = "";
  selectedTopic = "";
  
  Serial.println("Game initialized");
}

void generateRandomOptions() {
  // Generate 5 unique random genres
  bool used[sizeof(genres) / sizeof(genres[0])] = {false};
  for (int i = 0; i < 5; i++) {
    int randomIndex;
    do {
      randomIndex = random(0, sizeof(genres) / sizeof(genres[0]));
    } while (used[randomIndex]);
    used[randomIndex] = true;
    genreOptions[i] = genres[randomIndex];
  }
  
  // Generate 5 unique random topics
  bool topicUsed[sizeof(topics) / sizeof(topics[0])] = {false};
  for (int i = 0; i < 5; i++) {
    int randomIndex;
    do {
      randomIndex = random(0, sizeof(topics) / sizeof(topics[0]));
    } while (topicUsed[randomIndex]);
    topicUsed[randomIndex] = true;
    topicOptions[i] = topics[randomIndex];
  }
}

void showLoadingPage() {
  chinScreen_clear();
  currentState = STATE_LOADING;
  chinScreen_background_solid("black");
  
  // Fun animated loading screen (simplified - no blocking animations)
//  chinScreen_text("AI Adventure", 90, 80, "cyan", "cyoa-large");
//  chinScreen_text("Generator", 110, 120, "lime", "cyoa-large");
  lv_obj_t* theIcon = chinScreen_icon_white("cyoa", 1.0f, "middle", "center");
  lv_obj_add_event_cb(theIcon, restart_callback, LV_EVENT_CLICKED, NULL);
  
  
  chinScreen_text("Initializing...", 220, 420, "white", "cyoa-large");
  
  Serial.println("Loading page displayed");
}

void showGenreSelection() {
  chinScreen_clear();
  currentState = STATE_GENRE_SELECT;
  chinScreen_background_solid("black");
  
  generateRandomOptions();
  
  // Title
  chinScreen_text("Choose Your Genre:", 85, 20, "white", "cyoa-large");
  
  // Create 5 option buttons for genres
  optionAButton = chinScreen_button("darkblue", "white", 280, 35, genreOptions[0].c_str(), optionA_callback, "middle", "center", "small");
  lv_obj_set_pos(optionAButton, 0, -80);
  
  optionBButton = chinScreen_button("darkgreen", "white", 280, 35, genreOptions[1].c_str(), optionB_callback, "middle", "center", "small");
  lv_obj_set_pos(optionBButton, 0, -40);
  
  optionCButton = chinScreen_button("darkred", "white", 280, 35, genreOptions[2].c_str(), optionC_callback, "middle", "center", "small");
  lv_obj_set_pos(optionCButton, 0, 0);
  
  optionDButton = chinScreen_button("purple", "white", 280, 35, genreOptions[3].c_str(), optionD_callback, "middle", "center", "small");
  lv_obj_set_pos(optionDButton, 0, 40);
  
  optionEButton = chinScreen_button("orange", "white", 280, 35, genreOptions[4].c_str(), optionE_callback, "middle", "center", "small");
  lv_obj_set_pos(optionEButton, 0, 80);
  
  Serial.println("Genre selection displayed");
}

void showTopicSelection() {
  chinScreen_clear();
  currentState = STATE_TOPIC_SELECT;
  chinScreen_background_solid("black");
  
  // Title
  chinScreen_text("Choose Your Topic:", 80, 20, "white", "cyoa-large");
  
  // Create 5 option buttons for topics
  optionAButton = chinScreen_button("maroon", "yellow", 280, 35, topicOptions[0].c_str(), optionA_callback, "middle", "center", "small");
  lv_obj_set_pos(optionAButton, 0, -80);
  
  optionBButton = chinScreen_button("navy", "cyan", 280, 35, topicOptions[1].c_str(), optionB_callback, "middle", "center", "small");
  lv_obj_set_pos(optionBButton, 0, -40);
  
  optionCButton = chinScreen_button("darkgreen", "lime", 280, 35, topicOptions[2].c_str(), optionC_callback, "middle", "center", "small");
  lv_obj_set_pos(optionCButton, 0, 0);
  
  optionDButton = chinScreen_button("indigo", "white", 280, 35, topicOptions[3].c_str(), optionD_callback, "middle", "center", "small");
  lv_obj_set_pos(optionDButton, 0, 40);
  
  optionEButton = chinScreen_button("darkorange", "white", 280, 35, topicOptions[4].c_str(), optionE_callback, "middle", "center", "small");
  lv_obj_set_pos(optionEButton, 0, 80);
  
  Serial.println("Topic selection displayed");
}

void showSelectionTransition(String selectedOption, String nextPageText) {
  chinScreen_clear();
  currentState = STATE_SELECTION_TRANSITION;
  chinScreen_background_solid("black");
  
  // Show selected option in center
  chinScreen_text(selectedOption.c_str(), 160 - (selectedOption.length() * 6), 200, "cyan", "cyoa-large");
  
  // Show "next page loading" underneath
  chinScreen_text(nextPageText.c_str(), 160 - (nextPageText.length() * 5), 250, "yellow", "cyoa-large");
  
  // Add a small loading animation
  lv_obj_t* spinner = chinScreen_circle("white", "white", 10, "middle", "center");
  lv_obj_set_pos(spinner, 0, 50);
  chinScreen_animate_advanced(spinner, ANIM_ROTATE, 0, 360, 1000, true, "linear");
  
  Serial.printf("Selection transition: %s\n", selectedOption.c_str());
}

void showStoryPage() {
  chinScreen_clear();
  currentState = STATE_STORY;
  
  // Create black background
  chinScreen_background_solid("black");
  
  // Create story text with background rectangle for readability - adjusted for 320x480
  lv_obj_t* storyPanel = chinScreen_rectangle("black", "black", 320, 300, "top", "center");
 
  
  // Add story text inside panel
  bsp_display_lock(0);
  storyText = lv_label_create(storyPanel);
  lv_label_set_text(storyText, currentPage.text.c_str());
  lv_obj_set_style_text_color(storyText, lv_color_make(255, 255, 255), LV_PART_MAIN);
  lv_obj_set_style_text_font(storyText, getFontBySize("cyoa-large"), LV_PART_MAIN);
  lv_label_set_long_mode(storyText, LV_LABEL_LONG_WRAP);
  lv_obj_set_width(storyText, 280);
  lv_obj_align(storyText, LV_ALIGN_CENTER, 0, 0);
  bsp_display_unlock();
  
  // Check if game is over before showing options
  if (isGameOver()) {
    showGameOverPage();
  } else {
    showOptionsPage();
  }
  
  Serial.printf("Story page displayed: %s\n", currentPage.text.c_str());
}

void showOptionsPage() {
  currentState = STATE_OPTIONS;
  
  // Create option buttons stacked vertically in the middle - adjusted for 320x480
  optionAButton = chinScreen_button("black", "yellow", 280, 40, currentPage.option_a_text.c_str(), optionA_callback, "middle", "center", "small");
  lv_obj_set_pos(optionAButton, 0, 80);

  optionBButton = chinScreen_button("black", "blue", 280, 40, currentPage.option_b_text.c_str(), optionB_callback, "middle", "center", "small");
  lv_obj_set_pos(optionBButton, 0, 130);
  
  optionCButton = chinScreen_button("black", "green", 280, 40, currentPage.option_c_text.c_str(), optionC_callback, "middle", "center", "small");
  lv_obj_set_pos(optionCButton, 0, 180);
  
  // Add restart button as white refresh icon
  //lv_obj_t* restartIcon = chinScreen_icon_white("refresh", 0.8f, "bottom", "right");
  //lv_obj_add_event_cb(restartIcon, restart_callback, LV_EVENT_CLICKED, NULL);
  
  Serial.println("Options displayed");
}

void showGameOverPage() {
  currentState = STATE_GAMEOVER;
  
  // Remove any existing buttons
  if (optionAButton) {
    chinScreen_delete_object(optionAButton);
    optionAButton = nullptr;
  }
  if (optionBButton) {
    chinScreen_delete_object(optionBButton);
    optionBButton = nullptr;
  }
  if (optionCButton) {
    chinScreen_delete_object(optionCButton);
    optionCButton = nullptr;
  }
  
  // Show "GAME OVER" in red where buttons would be
  chinScreen_text("GAME OVER", 120, 300, "red", "cyoa-large");
  
  // Add restart icon
  //lv_obj_t* restartIcon = chinScreen_icon_white("refresh", 1.0f, "bottom", "center");
  //lv_obj_add_event_cb(restartIcon, restart_callback, LV_EVENT_CLICKED, NULL);
  
  Serial.println("Game Over page displayed");
}

bool isGameOver() {
  // Check if all three options contain "gameover" (case insensitive)
  String optA = currentPage.option_a_text;
  String optB = currentPage.option_b_text;
  String optC = currentPage.option_c_text;
  
  optA.toLowerCase();
  optB.toLowerCase();
  optC.toLowerCase();
  
  return (optA.indexOf("gameover") != -1 && 
          optB.indexOf("gameover") != -1 && 
          optC.indexOf("gameover") != -1);
}

void showErrorPage() {
  chinScreen_clear();
  currentState = STATE_ERROR;
  
  chinScreen_background_solid("darkred");
  
  chinScreen_text("Connection Error", 80, 100, "white", "large");
  chinScreen_text("Check LM Studio", 90, 150, "white", "medium");
  chinScreen_text("Server Running", 120, 180, "white", "medium");
  
  //lv_obj_t* retryIcon = chinScreen_icon_white("refresh", 1.0f, "bottom", "center");
  //lv_obj_add_event_cb(retryIcon, restart_callback, LV_EVENT_CLICKED, NULL);
  
  Serial.println("Error page displayed");
}

String fixJsonQuotes(String jsonString) {
  // Replace single quotes with double quotes, but be careful not to replace
  // single quotes that are inside string values
  jsonString.replace("{'", "{\"");
  jsonString.replace("':", "\":");
  jsonString.replace(": '", ": \"");
  jsonString.replace("', '", "\", \"");
  jsonString.replace("['", "[\"");
  jsonString.replace("', ", "\", ");
  jsonString.replace("'}", "\"}");
  jsonString.replace("']", "\"]");
  
  return jsonString;
}

bool requestNewStory(String userChoice) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return false;
  }
  
  // Show loading state
  if (currentState != STATE_LOADING) {
    // Update existing UI to show loading
    if (statusLabel) chinScreen_delete_object(statusLabel);
    chinScreen_text("Loading...", 220, 150, "yellow", "cyoa-large");
  }
  
  HTTPClient http;
  http.begin(LM_STUDIO_URL);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(30000); // 30 second timeout for local processing
  
  // Build the prompt with selected genre and topic
  String prompt = "You are a choose-your-own-adventure game master. Create a " + selectedGenre + " " + selectedTopic + 
           " adventure story.\n\nIMPORTANT: Respond ONLY with valid JSON using DOUBLE QUOTES like this exact format:\n" +
           "{\"page\": \"Your story text here\", \"options\": [\"Choice 1\", \"Choice 2\", \"Choice 3\"]}\n" +
           "Do not use single quotes. Do not add any text outside the JSON. Keep choices to 3 words or less. " +
           "Continue the story based on player choices. If the story ends, all 3 options should say 'GAMEOVER'.";

  if (userChoice.length() > 0) {
    prompt = userChoice + " - (REMEMBER TO ANSWER IN JSON WITH ONLY 2 FIELDS, TEXT AND OPTIONS - DO NOT USE SINGLE QUOTES AT ALL - IF THE GAME IS OVER, ALL 3 OPTIONS WILL SAY GAMEOVER)";
  }
  
  // Create JSON request for LM Studio
  DynamicJsonDocument requestDoc(2048);
  requestDoc["model"] = LM_STUDIO_MODEL;  // LM Studio uses whatever model is loaded
  requestDoc["temperature"] = 0.8;        // Add some creativity
  requestDoc["max_tokens"] = 500;         // Limit response length
  requestDoc["stream"] = false;           // Don't use streaming
  
  JsonArray messages = requestDoc.createNestedArray("messages");
  
  // Add conversation history if exists
  if (conversation_history.length() > 0) {
    JsonObject systemMsg = messages.createNestedObject();
    systemMsg["role"] = "system";
    systemMsg["content"] = "You are a choose-your-own-adventure game master. Always respond with valid JSON containing exactly two fields: 'page' (story text) and 'options' (array of 3 short choices).";
    
    JsonObject historyMsg = messages.createNestedObject();
    historyMsg["role"] = "assistant";
    historyMsg["content"] = conversation_history;
  }
  
  JsonObject userMsg = messages.createNestedObject();
  userMsg["role"] = "user";
  userMsg["content"] = prompt;
  
  String jsonString;
  serializeJson(requestDoc, jsonString);
  
  Serial.printf("Sending request to LM Studio: %s\n", jsonString.c_str());
  
  int httpResponseCode = http.POST(jsonString);
  
  if (httpResponseCode == 200) {
    String response = http.getString();
    Serial.printf("Response received: %s\n", response.c_str());
    
    // Parse response
    DynamicJsonDocument responseDoc(4096);
    deserializeJson(responseDoc, response);
    
    if (responseDoc.containsKey("choices") && responseDoc["choices"].size() > 0) {
      String content = responseDoc["choices"][0]["message"]["content"];
      parseGameResponse(content);
      conversation_history = content; // Store for context
      http.end();
      return true;
    }
  } else {
    Serial.printf("HTTP Error: %d\n", httpResponseCode);
    String errorResponse = http.getString();
    Serial.printf("Error response: %s\n", errorResponse.c_str());
    
    // Check if it's a connection error specifically
    if (httpResponseCode == -1) {
      Serial.println("Connection refused - is LM Studio server running?");
    }
  }
  
  http.end();
  return false;
}

void parseGameResponse(String jsonResponse) {
  Serial.printf("Parsing response: %s\n", jsonResponse.c_str());
  
  // Clean up the response - remove markdown code blocks
  jsonResponse.trim();
  if (jsonResponse.startsWith("```json")) {
    jsonResponse = jsonResponse.substring(7);
  }
  if (jsonResponse.startsWith("```")) {
    jsonResponse = jsonResponse.substring(3);
  }
  if (jsonResponse.endsWith("```")) {
    jsonResponse = jsonResponse.substring(0, jsonResponse.length() - 3);
  }
  jsonResponse.trim();
  
  // Fix quotes
  jsonResponse = fixJsonQuotes(jsonResponse);
  
  Serial.printf("Fixed JSON: %s\n", jsonResponse.c_str());
  
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, jsonResponse);
  
  if (error) {
    Serial.printf("JSON parsing failed: %s\n", error.c_str());
    Serial.println("Attempting fallback parsing...");
    
    if (jsonResponse.indexOf("\"page\":") > 0 || jsonResponse.indexOf("\"text\":") > 0) {
      parseGameResponseFallback(jsonResponse);
      return;
    }
    
    showErrorPage();
    return;
  }
  
  // Extract game data
  currentPage.page_id++;
  
  if (doc.containsKey("page")) {
    currentPage.text = doc["page"].as<String>();
    
    if (doc.containsKey("options")) {
      JsonArray options = doc["options"];
      if (options.size() >= 3) {
        currentPage.option_a_text = options[0].as<String>();
        currentPage.option_b_text = options[1].as<String>();
        currentPage.option_c_text = options[2].as<String>();
        
        currentPage.option_a_next = currentPage.page_id + 1;
        currentPage.option_b_next = currentPage.page_id + 2;
        currentPage.option_c_next = currentPage.page_id + 3;
      }
    }
  } else if (doc.containsKey("text")) {
    currentPage.text = doc["text"].as<String>();
    
    if (doc.containsKey("options")) {
      JsonArray options = doc["options"];
      if (options.size() >= 3) {
        currentPage.option_a_text = options[0].as<String>();
        currentPage.option_b_text = options[1].as<String>();
        currentPage.option_c_text = options[2].as<String>();
        
        currentPage.option_a_next = currentPage.page_id + 1;
        currentPage.option_b_next = currentPage.page_id + 2;
        currentPage.option_c_next = currentPage.page_id + 3;
      }
    }
  }
  
  if (currentPage.text.length() == 0) {
    currentPage.text = "Adventure continues...";
    currentPage.option_a_text = "Continue";
    currentPage.option_b_text = "Explore";
    currentPage.option_c_text = "Wait";
    currentPage.option_a_next = currentPage.page_id + 1;
    currentPage.option_b_next = currentPage.page_id + 2;
    currentPage.option_c_next = currentPage.page_id + 3;
  }
  
  showStoryPage();
}

// Button callbacks
void optionA_callback(lv_event_t* e) {
  Serial.println("Option A selected");
  
  if (currentState == STATE_GENRE_SELECT) {
    selectedGenre = genreOptions[0];
    Serial.printf("Genre selected: %s\n", selectedGenre.c_str());
    showTopicSelection();
  } else if (currentState == STATE_TOPIC_SELECT) {
    selectedTopic = topicOptions[0];
    Serial.printf("Topic selected: %s\n", selectedTopic.c_str());
    Serial.printf("Starting game with: %s %s\n", selectedGenre.c_str(), selectedTopic.c_str());
    currentState = STATE_WAITING;
    requestNewStory();
  } else {
    currentState = STATE_WAITING;
    requestNewStory(currentPage.option_a_text);
  }
}

void optionB_callback(lv_event_t* e) {
  Serial.println("Option B selected");
  
  if (currentState == STATE_GENRE_SELECT) {
    selectedGenre = genreOptions[1];
    Serial.printf("Genre selected: %s\n", selectedGenre.c_str());
    showTopicSelection();
  } else if (currentState == STATE_TOPIC_SELECT) {
    selectedTopic = topicOptions[1];
    Serial.printf("Topic selected: %s\n", selectedTopic.c_str());
    Serial.printf("Starting game with: %s %s\n", selectedGenre.c_str(), selectedTopic.c_str());
    currentState = STATE_WAITING;
    requestNewStory();
  } else {
    currentState = STATE_WAITING;
    requestNewStory(currentPage.option_b_text);
  }
}

void optionC_callback(lv_event_t* e) {
  Serial.println("Option C selected");
  
  if (currentState == STATE_GENRE_SELECT) {
    selectedGenre = genreOptions[2];
    Serial.printf("Genre selected: %s\n", selectedGenre.c_str());
    showTopicSelection();
  } else if (currentState == STATE_TOPIC_SELECT) {
    selectedTopic = topicOptions[2];
    Serial.printf("Topic selected: %s\n", selectedTopic.c_str());
    Serial.printf("Starting game with: %s %s\n", selectedGenre.c_str(), selectedTopic.c_str());
    currentState = STATE_WAITING;
    requestNewStory();
  } else {
    currentState = STATE_WAITING;
    requestNewStory(currentPage.option_c_text);
  }
}

void optionD_callback(lv_event_t* e) {
  Serial.println("Option D selected");
  
  if (currentState == STATE_GENRE_SELECT) {
    selectedGenre = genreOptions[3];
    Serial.printf("Genre selected: %s\n", selectedGenre.c_str());
    showTopicSelection();
  } else if (currentState == STATE_TOPIC_SELECT) {
    selectedTopic = topicOptions[3];
    Serial.printf("Topic selected: %s\n", selectedTopic.c_str());
    Serial.printf("Starting game with: %s %s\n", selectedGenre.c_str(), selectedTopic.c_str());
    currentState = STATE_WAITING;
    requestNewStory();
  }
}

void optionE_callback(lv_event_t* e) {
  Serial.println("Option E selected");
  
  if (currentState == STATE_GENRE_SELECT) {
    selectedGenre = genreOptions[4];
    Serial.printf("Genre selected: %s\n", selectedGenre.c_str());
    showTopicSelection();
  } else if (currentState == STATE_TOPIC_SELECT) {
    selectedTopic = topicOptions[4];
    Serial.printf("Topic selected: %s\n", selectedTopic.c_str());
    Serial.printf("Starting game with: %s %s\n", selectedGenre.c_str(), selectedTopic.c_str());
    currentState = STATE_WAITING;
    requestNewStory();
  }
}

void restart_callback(lv_event_t* e) {
  Serial.println("Restarting game");
  conversation_history = "";
  setupGame();
  chinScreen_background_solid("black");
  showLoadingPage();
  delay(1000);
  
  // Test connection again before showing genre selection
  if (testLMStudioConnection()) {
    delay(1000); // Give time for loading screen
    showGenreSelection();
  } else {
    showErrorPage();
  }
}

// Fallback parser for when JSON is malformed but contains readable data
void parseGameResponseFallback(String response) {
  Serial.println("Using fallback parser");
  
  // Extract text between quotes after "page": or "text":
  int textStart = response.indexOf("\"page\":") + 8;
  if (textStart <= 7) {
    textStart = response.indexOf("\"text\":") + 8;
  }
  
  if (textStart > 7) {
    int textEnd = response.indexOf("\",", textStart);
    if (textEnd == -1) textEnd = response.indexOf("\"", textStart + 1);
    
    if (textEnd > textStart) {
      currentPage.text = response.substring(textStart, textEnd);
      currentPage.text.replace("\\\"", "\""); // Unescape quotes
    }
  }
  
  // Extract options array
  int optionsStart = response.indexOf("\"options\":");
  if (optionsStart > 0) {
    int arrayStart = response.indexOf("[", optionsStart);
    int arrayEnd = response.indexOf("]", arrayStart);
    
    if (arrayStart > 0 && arrayEnd > arrayStart) {
      String optionsArray = response.substring(arrayStart + 1, arrayEnd);
      
      // Simple parsing for ["A: text", "B: text", "C: text"] format
      int firstComma = optionsArray.indexOf(",");
      int secondComma = optionsArray.indexOf(",", firstComma + 1);
      
      if (firstComma > 0 && secondComma > firstComma) {
        String opt1 = optionsArray.substring(1, firstComma - 1); // Remove quotes
        String opt2 = optionsArray.substring(firstComma + 3, secondComma - 1);
        String opt3 = optionsArray.substring(secondComma + 3, optionsArray.length() - 1);
        
        // Remove "A: ", "B: ", "C: " prefixes if they exist
        if (opt1.indexOf(":") > 0) {
          currentPage.option_a_text = opt1.substring(opt1.indexOf(":") + 2);
        } else {
          currentPage.option_a_text = opt1;
        }
        
        if (opt2.indexOf(":") > 0) {
          currentPage.option_b_text = opt2.substring(opt2.indexOf(":") + 2);
        } else {
          currentPage.option_b_text = opt2;
        }
        
        if (opt3.indexOf(":") > 0) {
          currentPage.option_c_text = opt3.substring(opt3.indexOf(":") + 2);
        } else {
          currentPage.option_c_text = opt3;
        }
        
        currentPage.page_id++;
        currentPage.option_a_next = currentPage.page_id + 1;
        currentPage.option_b_next = currentPage.page_id + 2;
        currentPage.option_c_next = currentPage.page_id + 3;
        
        showStoryPage();
        return;
      }
    }
  }
  
  // If fallback fails, show error
  Serial.println("Fallback parsing also failed");
  showErrorPage();
}
