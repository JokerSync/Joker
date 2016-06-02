# DetX XML specification

The *DetX* is divided into three part:

- The `<header>` tag holds general information about the document:
	- `<cappella>` : This tag contains information about the generator
		- `"copyright"`
		- `"version"`
	- `<title>` : Original title
	- `<title2>` : Translated title (optional)
	* `<episode>` : Episode information
		* `"number"` (mandatory)
		* `"season"` (mandatory)
		* `"title"` (mandatory)
		* `"title2"` : Translated title (optional)
	* `<author>` : Author information
		* `"name"` (mandatory)
		* `"firstname"` (mandatory)
		* `"email"` : adresse de courrier électronique  [optionnel]
	* `<detector>` : 
		* `"name"` (mandatory)
		* `"firstname"` (mandatory)
		* `"email"` : adresse de courrier électronique  [optionnel]
	* `<production>` :
		* `"producer"` : Producteur [optionnel]
		* `"year"` : Année de production [optionnel]
		* `"country"` : Pays d'origine[optionnel]
		* `"director"` : Réalisateur [optionnel]
		* `"distributor"` : Distributeur [optionnel]
		* `"diffuser"` : Diffuseur [optionnel]
	* `<notes>` : Notes diverses à l'intention du directeur artistique [élément optionnel]
	* `<summary>` : Résumé [élément optionnel]
	* `<videofile>` : Emplacement du fichier vidéo correspondant
	* `<audiofile>` : Emplacement du fichier audio correspondant [élément optionnel]
	* `<detectionfile>` : Emplacement du fichier contenant la détection [élément optionnel]
	
	* `<last_position>` : This tag give the state of the software when the document was saved:
		* `"timecode"`
		* `"track"`

* The `<roles>` tag holds the list of characters in the document. Each `<role>` tag contains the following attributes:
	* `"name"` (mandatory)
	* `"id"` (mandatory)
	* `"gender"` : `"male"`, `"female"` or `"neutral"` (default)
	* `"color"` : text color on the rythmo band using HTML format `"#RRGGBB"` (default = `"#000000")
	* `"description"` : Description du personnage [optionnel]

The `<role>` tag can also contain an optional sub tag `<image>` containing a JPEG base 64 encoded picture.

* The `<body>` tag contains the rythmo band description
	* `<loop>` indicates a loop change
	* `<shot>` indicates a cut change
	* `<line>` indicates a sentence

* `<line>` describe a sentence
	* The `"role"` attribute contains the corresponding `<role>` unique id.
	* The `"voice"` attribute indicates if the sentence is spoken `"on"` or `"off"` (default) the picture. It can also be indique `"semi_off".
	* The `"track"` attribute indicates the vertical position of the sentence from 0 (default) to 3.
	* The `"type"` attribute indicates if it is a `"normal"` phrase (default) or a `"reac"`.
	* The `<line>` tag can contain `<lipsync>` and `<text>`

* The `<lipsync>` tag describe a detection sign:
	* The `"type"` attribute specify its nature:
		* start (`"in_open"`, `"in_close"`)
		* end (`"out_open"`, `"out_close"`)
		* inner lip sync: `"mpb"`, `"fvr"`, `"weuqueu"`, `"a"`, `"i"`, `"o"`, `"neutral"` (default)
	* The `"timecode"` or `"tc"` indicate its temporal position.
	* The `"link"` attribute indicates if the lipsync in linked to its surrounding text ("on": default) or not ("off).

* The `<text>` contains a text string and optional carriage return (`<br/>`).

The `"timecode"` or `"tc"` of the `<loop>`, `<shot>` and `<lipsync>` tags indicate the temporal position using the following format HH:MM:SS:FF (FF for frame). These tags are ordered by ascending `"timecode"` in the file.
