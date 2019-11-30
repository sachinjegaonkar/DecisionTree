var txtVersion = "1.1";
var addtoInterval = null;
var popupWin = '';

var sns = [
	 {name:'Del.icio.us',	id: 2,  image: 'AddTo_Delicious.gif'},
	 {name:'Digg',			id: 3,  image: 'AddTo_Digg.gif'},
	 {name:'Google',		id: 5,  image: 'AddTo_Google.gif'},
	 {name:'Spurl',			id: 8,  image: 'AddTo_Spurl.gif'},
	 {name:'Blink',			id: 1,  image: 'AddTo_Blink.gif'},
	 {name:'Furl',			id: 4,  image: 'AddTo_Furl.gif'},
	 {name:'Simpy',			id: 6,  image: 'AddTo_Simpy.gif'},
	 {name:'Yahoo! MyWeb',	id: 7,  image: 'AddTo_Yahoo.gif'}
];

function DrawLink(index, cellClass)
{
	document.write("<span class=\"" + cellClass + "\" title=\"Add this page to " + sns[index].name + "\" ");
	document.write("onclick=\"addto(" + sns[index].id.toString() + ")\"><img align=\"absmiddle\" src=\"/images/" + sns[index].image + "\" ");
	document.write("width=\"16\" height=\"16\" border=\"0\" /> " + sns[index].name + "</span>");
}

function DrawLinks(cols, width, headClass, cellClass) 
{
	if (cols > sns.length) 
	{
		document.write("<span class=\"" + headClass + "\">Add this article to: </span> ")
		for (var i = 0; i < sns.length; i++)
		{
			DrawLink(i, cellClass)
			document.write(" ");
		}
	} else {
		document.writeln("<table width=\"" + width.toString() + "\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr>");
		document.writeln("<tr><td colspan=\"" + cols.toString() + "\" class=\"" + headClass + "\" height=\"20\">Add this article to:</td></tr>");
		for (var i = 0; i < sns.length; i++)
		{
			document.write("<td valign=\"middle\">");
			DrawLink(i, cellClass);
			document.writeln("</td>");
			if ( (((i+1)%cols) == 0) && (i != (sns.length-1))) document.writeln("</tr><tr>");
		}
		document.writeln("</td></tr></table>");
	}
}

function addtoWin(addtoFullURL)
{
	if (!popupWin.closed && popupWin.location){
		popupWin.location.href = addtoFullURL;
		var addtoInterval = setInterval("closeAddTo();",1000);
	}
	else{
		popupWin = window.open(addtoFullURL,'addtoPopUp','width=770px,height=500px,menubar=1,toolbar=1,status=1,location=1,resizable=1,scrollbars=1,left=0,top=100');
		var addtoInterval = setInterval("closeAddTo();",1000);
		if (!popupWin.opener) popupWin.opener = self;
	}
	if (window.focus) {popupWin.focus()}
	return false;
}
// closes the popupWin
function closeAddTo() {
	if (!popupWin.closed && popupWin.location){
		if (popupWin.location.href == AddURL)	//if it's the same url as what was bookmarked, close the win
		popupWin.close();
	}
	else {	//if it's closed - clear the timer
		clearInterval(addtoInterval)
		return true
	}
}
//main addto function - sets the variables for each Social Bookmarking site
function addto(addsite){
	switch(addsite){
		case 1:	//	Blink ID:1
			var AddSite = "http://www.blinklist.com/index.php?Action=Blink/addblink.php";
			var AddUrlVar = "url";
			var AddTitleVar = "title";
			var AddNoteVar = "description";
			var AddReturnVar = "";
			var AddOtherVars = "&Action=Blink/addblink.php";	
			break
		case 2:	//	Del.icio.us	ID:2 &v=3&noui=yes&jump=close
			var AddSite = "http://del.icio.us/post?";
			var AddUrlVar = "url";
			var AddTitleVar = "title";
			var AddNoteVar = "";
			var AddReturnVar = "";
			var AddOtherVars = "";		
			break
		case 3:	//	Digg ID:3
			var AddSite = "http://digg.com/submit?";
			var AddUrlVar = "url";
			var AddTitleVar =  "";
			var AddNoteVar =  "";
			var AddReturnVar =  "";
			var AddOtherVars = "&phase=2";
			break
		case 4:	//	Furl ID:4
			var AddSite = "http://www.furl.net/storeIt.jsp?";
			var AddUrlVar = "u";
			var AddTitleVar = "t";
			var AddNoteVar = "";
			var AddReturnVar = "";
			var AddOtherVars = "";	
			break
		case 5:	//	GOOGLE ID:5
			var AddSite = "http://fusion.google.com/add?";
			var AddUrlVar = "feedurl";
			var AddTitleVar = "";
			var AddNoteVar = "";
			var AddReturnVar = "";
			var AddOtherVars = "";
			break
		case 6:	//	Simpy ID:6
			var AddSite = "http://simpy.com/simpy/LinkAdd.do?";
			var AddUrlVar = "href";
			var AddTitleVar = "title";
			var AddNoteVar = "note";
			var AddReturnVar = "_doneURI";
			var AddOtherVars = "&v=6&src=bookmarklet";
			break
		case 7:	//	Yahoo ID: 7
			var AddSite = "http://myweb2.search.yahoo.com/myresults/bookmarklet?";
			var AddUrlVar = "u";
			var AddTitleVar = "t";
			var AddNoteVar = "";
			var AddReturnVar = "";
			var AddOtherVars = "&d=&ei=UTF-8";
			break
		case 8:	//	Spurl ID: 8 	d.selection?d.selection.createRange().text:d.getSelection()
			var AddSite = "http://www.spurl.net/spurl.php?";
			var AddUrlVar = "url";
			var AddTitleVar = "title";
			var AddNoteVar = "blocked";
			var AddReturnVar = "";
			var AddOtherVars = "&v=3";
			break
			//	To add more bookmarking sites, find the posting URL, identify the variable names, and create another case statement
		default:
	}
//	Build the URL
	var addtoFullURL = AddSite + AddUrlVar + "=" + AddURL + "&" + AddTitleVar + "=" + AddTitle + AddOtherVars ;
	if (AddNoteVar != "") 
		{var addtoFullURL = addtoFullURL + "&" + AddNoteVar + "=" + AddTitle;}
	if (AddReturnVar != "")
		{var addtoFullURL = addtoFullURL + "&" + AddReturnVar + "=" + AddURL;}
//	Checking AddToMethod, to see if it opens in new window or not
	switch(addtoMethod){
		case 0:	// 0=direct link
			self.location = addtoFullURL
			break
		case 1:	// 1=popup
			addtoWin(addtoFullURL);
			break
		default:	
		}
		return true;
}
//	checking across domains causes errors - this is to supress these
function handleError() {return true;}
window.onerror = handleError;