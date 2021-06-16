"use strict"

	// Makes a basic page and title
	let login_body = document.getElementsByTagName('body')[0];
	let bolden = document.createElement('b');
	let line_break3 = document.createElement("br");
	let line_break4 = document.createElement("br");
	let head_one = document.createElement('h1');
	let title = document.createTextNode("Skills Test");
	let instructions = document.createTextNode("Input your name, a hobby and a picture.");
	bolden.appendChild(title);
	head_one.appendChild(bolden);
	login_body.appendChild(head_one);
	login_body.appendChild(line_break3);
	login_body.appendChild(instructions);
	login_body.appendChild(line_break4);


	// Makes the form elements
	let page_form = document.createElement("FORM");
	let line_break1 = document.createElement("br");
	let line_break2 = document.createElement("br");
	let line_break5 = document.createElement("br");
	let line_break6 = document.createElement("br");
	let first_num = document.createTextNode("Name: ");
	let second_num = document.createTextNode("Hobby: ");
	let first_input = document.createElement("input");
	let second_input = document.createElement("input");
	let add_button = document.createElement("BUTTON");
	let button_text = document.createTextNode("See Table");
	let attach_button = document.createElement("input");
	attach_button.setAttribute("type","file");

	// adds all the form elements into the form
	page_form.appendChild(first_num);
	page_form.appendChild(first_input);
	page_form.appendChild(line_break1);
	page_form.appendChild(second_num);
	page_form.appendChild(second_input);
	page_form.appendChild(line_break2);
	page_form.appendChild(attach_button);
	page_form.appendChild(line_break5);
	page_form.appendChild(line_break6);
	add_button.appendChild(button_text);

	first_input.id = 'name';
	second_input.id = 'hobby';
	attach_button.id = 'picture';

	let callback = function() {
		alert('Data sent.');
    	let querystring = "table.html";
		let xhr = new XMLHttpRequest();
		// successful data submission
  		xhr.addEventListener('load', function(event) {
    		login_body.innerHTML = this.response;
  		});
 		// error
  		xhr.addEventListener('error', function(event) {
    		alert('Oops! Something went wrong.');
  		});
  		xhr.open('GET', 'table.html');
  		xhr.send();
	}

	let join_server = function(){
		let name_text = document.getElementById('name');
		let hobby_text = document.getElementById('hobby');
		let picture_file = document.getElementById('picture');

		let XHR = new XMLHttpRequest();

		//var urlEncodedData = '';
		//var urlEncodedDataPairs = [];

		// Turn the data into an array of URL-encoded key/value pairs
		//urlEncodedDataPairs.push(encodeURIComponent('name') + '=' + encodeURIComponent(name_text));
		//urlEncodedDataPairs.push(encodeURIComponent('hobby') + '=' + encodeURIComponent(hobby_text));
		//urlEncodedDataPairs.push(encodeURIComponent('picture') + '=' + encodeURIComponent(picture_file));
		// combine pairs into string and replace % w/ '+'.
		// Matches behavior of browser form submissions.
		//urlEncodedData = urlEncodedDataPairs.join('&').replace(/%20g/, '+');
		// successful data submission
  		XHR.addEventListener('load', function(event) {

  		});
  		// error
  		XHR.addEventListener('error', function(event) {
    		alert('Oops! Something went wrong.');
  		});
  		// Set up our request
  		XHR.open('POST', '');

  		// Add the required HTTP header for form data POST requests
  		XHR.setRequestHeader('Content-Type', 'application/json');
  		// Finally, send our data.
  		XHR.send(JSON.stringify({
  			'name':name_text,
  			'hobby':hobby_text,
  			'picture':picture_file
  		}));

	}

	add_button.onclick = join_server;
	login_body.appendChild(page_form);
	login_body.appendChild(add_button);












