const mysql = require('mysql');
const pg = require('pg');
const express = require('express');
const session = require('express-session');
const path = require('path');
const fs = require('fs');

/*const connection = mysql.createConnection({
	host     : 'localhost',
	user     : 'root',
	password : 'Welcome@1',
	database : 'nodelogin'
}); */
const conString = "postgres://I064926:@localhost:5432/I064926";
const connection = new pg.Client(conString);
connection.connect();

const app = express();

app.use(session({
	secret: 'secret',
	resave: true,
	saveUninitialized: true
}));
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname, 'static')));

// http://localhost:3000/
app.get('/', function(request, response) {
	// Render login template
	response.sendFile(path.join(__dirname + '/login.html'));
});

app.get('/carControl', function(request, response) {
	// Render login template
	//response.sendFile(path.join(__dirname + '/filesList.html'));
	response.sendFile(path.join(__dirname + '/rcControl.html'));
});

// http://localhost:3000/auth
app.post('/auth', function(request, response) {
	// Capture the input fields
	let username = request.body.username;
	let password = request.body.password;
	// Ensure the input fields exists and are not empty
	if (username && password) {
		console.log(username);
		console.log(password);
		const query = `SELECT * FROM accounts WHERE username = '${username}' AND password = '${password}'`; // Vulnerable to SQL injection
		// Execute SQL query that'll select the account from the database based on the specified username and password
		connection.query(query, function(error, results) {
			// If there is an issue with the query, output the error
			if (error) throw error;
			console.log('result from select statement');
			console.log(results.rows.length);
			// If the account exists
			if (results.rows.length > 0) {
				// Authenticate the user
				request.session.loggedin = true;
				request.session.username = username;
				// Redirect to home page
				response.redirect('/carControl');
			}else{
				response.send('Incorrect Username and/or Password!');
			}
			response.end();
		});
	} else {
		response.send('Please enter Username and Password!');
		response.end();
	}
});

// http://localhost:3000/home
app.get('/home', function(request, response) {
	// If the user is loggedin
	if (request.session.loggedin) {
		// Output username
		response.send('Welcome back, ' + request.session.username + '!');
	} else {
		// Not logged in
		response.send('Please login to view this page!');
	}
	response.end();
});

const port = 3000;
app.listen(port, () => console.log(`Server listening on port ${port}`));