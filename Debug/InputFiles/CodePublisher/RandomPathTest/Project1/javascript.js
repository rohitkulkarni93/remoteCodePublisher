function showHideSection(division) { var x = document.getElementById(division);
if (x.style.display == 'none') {
x.style.display = 'inline'; }
else {
x.style.display = 'none'; 
} 
}


alert('loaded');function showHideElement(i) {	if (document.getElementById(i).style.visibility == 'hidden')		document.getElementById(i).style.visibility = 'visible';	else		document.getElementById(i).style.visibility = 'hidden';}