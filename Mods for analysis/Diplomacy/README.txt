===========================================================================
 REPUTATION 2.0 - Diplomacy, espionage and the people
 for Workers & Resources: Soviet Republic  (1.1.1.9)
 Requires TesmioLoader
===========================================================================

Three things in this file:

   1.  INSTALLING              two steps, and Steam cannot do them for you
   2.  CHANGING THINGS         every number in this mod is text you can edit
   3.  ADDING YOUR OWN EVENTS  without touching the mod's own file

There is a fourth thing worth knowing before any of it: THE CHEATS WINDOW.
Open the Diplomacy panel in the left sidebar, go to Cheats, and you can hand
yourself standing, money, intelligence, licences, blueprints and research.
That is not just for testing. If part of this mod is not the part you wanted
to play, the cheats window is how you skip it - take the standing and go and
do the bit you came for. Nobody is marking your homework.


===========================================================================
 1.  INSTALLING
===========================================================================

STEP ONE. Copy the "tesmioloader" folder that sits beside this file into:

    ...\steamapps\common\SovietRepublic\

Say YES to merging. The files land where every other loader plugin's do:

    tesmioloader\build\plugins\reputation.dll
    tesmioloader\build\plugins\reputation.ini
    tesmioloader\build\plugins\resources.ini
    tesmioloader\build\plugins\reputation.d\example-mod-events.ini
    tesmioloader\vfs\media_soviet\resources\small_arms.png
    tesmioloader\vfs\media_soviet\resources\large_arms.png
    tesmioloader\vfs\media_soviet\resources\munitions.png

Steam cannot do this for you. TesmioLoader loads plugins out of its own
folder, which Steam does not write to. Every loader plugin is installed by
hand; this one is no different.

STEP TWO IS DONE BY STEP ONE. resources.ini ships in the same folder, with
the three weapons resources (small_arms, large_arms, munitions) already in it.
When Windows asks, choose REPLACE the files in the destination. You need the
resources plugin (resources.dll) installed.

Replacing it overwrites any resources another mod put in your resources.ini.
If you use such a mod, put its lines back afterwards.

Without these resources, hovering a weapons factory in the build menu crashes
the game. They are part of the save format: start a new republic after
installing.

Then launch through tesmiolauncher.exe, not Steam's Play button.

DID IT WORK? tesmioloader.log should contain:

    plugin   reputation       2.0 from reputation.dll

And there will be a new entry in the left sidebar called Diplomacy.

THE BUILDINGS ARE A SEPARATE WORKSHOP ITEM. The mod runs without them, but
the embassies and the two headquarters are how most of it is reached.

NO REQUIRED ITEMS. The small Munitions Plant uses Kalmoire's "Pollena Uroda
Chemical Products Factory" (3172083143); since 21 Sep 2026 Kalmoire has given
full permission and the model ships inside the buildings item, so nothing
else needs subscribing to.


===========================================================================
 2.  CHANGING THINGS
===========================================================================

Everything this mod does lives in

    tesmioloader\build\plugins\reputation.ini

as plain text with a comment above every setting explaining what it does and
why it is the number it is. Edit it in Notepad. It is read when the game
starts, so save the file and restart the game.

TWO RULES, AND THE FIRST ONE HAS COST PEOPLE AN EVENING:

  * SAVE AS UTF-8 WITHOUT A BOM. Notepad does this correctly. PowerShell's
    -Encoding UTF8 does NOT - it adds a BOM, the first section header stops
    matching, and every setting in the file silently falls back to its
    default. Nothing is logged. Nothing looks wrong.

  * A "list =" line must stay ONE line. There is no continuation character.

If something is wrong the plugin says so in

    tesmioloader\build\reputation.log

That file is meant to be read. It says what it found, what it changed, and
what it refused and why.


--- TURNING WHOLE FEATURES OFF ------------------------------------------

Do not like a part of it? Switch it off rather than fighting it.

  [reputation]  enabled = 0          the whole plugin, off
  [population]  find_citizens = 0    no citizen reading at all (see below)
  [population]  apply = 0            read the people, never write to them
  [people]      enabled = 0          the people stop affecting your republic
  [intel]       enemy_operations = 0 they stop running operations against you
  [cheats]      enable = 0           hide the cheats window
  [ui]          in_panel = 0         old-style floating windows instead


--- THE SETTINGS PEOPLE ACTUALLY CHANGE ---------------------------------

  [standing]
    start             = 20      what a new republic begins with
    max               = 100     the ceiling
    min               = -50     the floor, where you get blockaded
    decay_per_day     = 0.01    how fast standing bleeds away on its own
    decay_floor       = 10      below this, decay stops - indifference, not
                                hostility
    Turn decay_per_day down if you want a gentler game, or up to 0.02 if
    standing feels too easy to hold.

  [population]
    find_citizens = 1     find the real people in memory after a save loads.
                          Costs ONE pause per load. Turn it off and you lose
                          the Espionage HQ's effect, the population line and
                          every event that asks about the people.
    apply         = 1     let buildings and events actually move them
    step_per_day  = 0.01  how fast a citizen moves towards a target. 0.02 is
                          twice as responsive and twice as noticeable
    hard_cap      = 0.95  loyalty can never be pushed past this
    recovery_days = 120   how long an effect takes to wear off afterwards

  [people]          what your population does back to your republic
    loyalty_to_pay    = 0.30    loyal republic, better paid contracts
    crime_to_pay      = 2.00    crime takes it off the top
    loyalty_to_soviet = 0.030   per day, Moscow warms or cools
    crime_to_west     = 0.200   per day, the West reads the reports
    crime_to_risk     = 1.00    lawless republics leak spies
    All of them are zero until the citizens have been found, and all are
    measured from YOUR republic's own starting average.

  [ui]
    panel_title  = Diplomacy    what the red band above the panel says
    font_size    = 30           30 or 22. Everything scales with it
    in_panel     = 1            draw inside the sidebar panel
    tab_group_names = Embassies, Foreign relations, Espionage, Small arms,
                      Munitions, Heavy weapons
                      the headings inside the bottom construction tab, one
                      per group of tab_buildings, in the same order. Rename
                      them to whatever you like.
    engine_strings = 1
                      the mod has no strings of its own in the game's table -
                      there is no "Diplomacy" or "Munitions" in it - so it
                      asks the engine to hold them, the same way a workshop
                      author's own country name is held. That is what makes
                      the hover tooltips read correctly as well as the
                      headings. Set it to 0 if a label ever comes out as
                      somebody else's word, and set tab_string_id = 0x187A
                      to go all the way back to the game's own "Secret
                      police".

  [keys]        every shortcut, rebindable. Codes are Windows virtual-key
                numbers in hex - 0x78 is F9, 0x76 is F7, and so on.

  [intel]
    enemy_chance      = 0.004   how often they come after you
    risk_cap          = 0.85    the worst an operation's risk can get
    officer_cap       = 12      how many officers you may have
    train_cost_rub    = 400000  what one costs to train

MAKING EVENTS RARER OR MORE COMMON. Every event has a "chance" line - a
probability per day. 0.001 is roughly one year in three. Halve them all and
the game gets quiet; double them and it gets eventful.


===========================================================================
 3.  ADDING YOUR OWN EVENTS
===========================================================================

You do NOT edit reputation.ini to add content. Drop your own .ini into

    tesmioloader\build\plugins\reputation.d\

and the plugin reads it exactly as it reads its own file, on top of its own
content. An update to the mod will not overwrite your work.

There is a worked example already in that folder: example-mod-events.ini.
Copy it, rename it to whatever your mod is called, and edit. The FILE name
does not matter - only the section names do.

THE RULES:

  * Your content is ADDED, not substituted. The mod's own 89 events stay.
  * An id somebody else already used is REFUSED, not overridden, and the log
    names the file that was ignored. So put a prefix on yours.
  * UTF-8, no BOM. "list =" on one line. Same as above.

THE SHAPE OF AN EVENT:

    [events]
    list = mymod_harvest_failure

    ; A bad harvest. Rolls once every few years between 1950 and 1975 and
    ; runs for a season when it does.
    [mymod_harvest_failure]
    name              = The harvest failed
    blurb             = Rain in August, and the fields are still standing.
    chance            = 0.0008     ; per day. 0.001 is about 1 year in 3
    from_year         = 1950
    to_year           = 1975
    duration          = 150        ; days it runs
    once              = 0          ; 1 = at most once per republic
    category          = disaster   ; one event per category at a time
    category_cooldown = 400
    soviet_once       = -3         ; standing, the day it lands
    rub_once          = -0.05      ; -0.05 = five per cent of the treasury
    prices            = food:1.6:1.4, meat:1.5:1.3
    status_bonus      = satiated:-0.06, happiness:-0.04

    ; Or a dated one, which fires on its date and never rolls:
    [mymod_the_congress]
    name              = The nineteenth congress
    start_year        = 1952
    start_day         = 279
    duration          = 200
    once              = 1
    status_bonus      = loyalty:0.05
    soviet_once       = 4

WHAT AN EVENT CAN CARRY:

  WHEN     chance, from_year, to_year, start_year, start_day, duration,
           once, needs_soviet, needs_west, category, category_cooldown,
           prevented_by, prevent_chance, needs_event, after_event,
           min_loyalty, max_loyalty, min_crime, max_crime

  MONEY    rub_once, usd_once, rub_per_day, usd_per_day
           a "once" value between -1 and 1 is a FRACTION of the treasury;
           anything bigger is a flat sum

  STANDING soviet_once, west_once, soviet_per_day, west_per_day

  TRADE    prices (up to six, "resource:roubles[:dollars]")
           block_import, block_export, embargo  (none|soviet|west|both)
           contract_pay_mult, contract_standing_mult, decay_multiplier,
           arms_penalty_mult

  PEOPLE   status_bonus = happiness:-0.08, religion:0.05
           crime_multiplier = 1.6

THE NINE STATUSES, by the names status_bonus uses:

    happiness   satiated   health   loyalty   alcohol
    culture     sports     religion   clothing

  The value is DISTANCE FROM YOUR REPUBLIC'S OWN AVERAGE, not an absolute.
  happiness:-0.08 aims eight points below whatever your republic's happiness
  actually is. Positive pushes up, negative pushes down, absent leaves it
  alone. Keep it small: 0.03 is visible, 0.10 is enormous.

  crime_multiplier below 1.0 holds crime DOWN to that fraction of your
  republic's own rate. Above 1.0 is a crime wave. The game fights an upward
  push, so a wave reaches less than it asks for and fades when the event
  ends - which is what a crime wave is.

A CATEGORY IS A QUEUE. Events sharing a category name cannot run at the same
time, and the one that fires sets the lockout for the rest. That is what
stops a famine, a scandal and a border incident landing in the same
fortnight. Pick a new category name for your own events rather than joining
a crowded one.

YOU CAN ALSO ADD, the same way, in the same file: contracts, grants,
treaties, vehicle licence tiers, blueprints, espionage operations,
diplomacy actions and institutions. Every section in reputation.ini is
documented in place - find the one you want, copy its shape, and change the
name.

AND IF YOU ARE WRITING A PLUGIN of your own: this one publishes a service
you can consume - standing, events, contracts, the calendar, the population,
and a call to fire one of this mod's events from your code. The header is
reputation_api.h in the github repo.


===========================================================================
 PROBLEMS
===========================================================================

Read tesmioloader\build\reputation.log first. It is written to be read.

Then: https://github.com/daniepe/Reputation-Plugin-for-Workers-and-Resources

Source is GPL-3.0. Workers & Resources: Soviet Republic and its assets
remain the property of their owners; this is an independent fan modification
and is not affiliated with or endorsed by 3Division or Hooded Horse.
