CREATE DATABASE IF NOT EXISTS Cloudisk;

USE Cloudisk;

DROP TABLE IF EXISTS User;
CREATE TABLE User
(
	username VARCHAR(20) PRIMARY KEY,
	password CHAR(32) NOT NULL,
	pwd VARCHAR(128) 
);

DROP TABLE IF EXISTS File;
CREATE TABLE File
(
	fid INT PRIMARY KEY AUTO_INCREMENT,
	filename VARCHAR(255) NOT NULL,
	filetype VARCHAR(10) NOT NULL,
	username VARCHAR(20) NOT NULL,
	filepath VARCHAR(128) NOT NULL,
	CONSTRAINT FOREIGN KEY (username) REFERENCES User(username) ON DELETE CASCADE
);

DROP TABLE IF EXISTS GlobalFile;
CREATE TABLE GlobalFile
(
	file_hash CHAR(32) PRIMARY KEY,
	filename VARCHAR(255) NOT NULL,
	filesize BIGINT NOT NULL
);

UPDATE User set pwd = '/' where username = 'ccxts';

INSERT INTO User VALUES
('ccxts', 'JNBHJASDKNJNKAFD5S8ERAU7S8UJ18SJ', NULL);

INSERT INTO File(filename, filetype, username, filepath) VALUES
('text1', 'f', 'ccxts', 'ccxts/'),
('text2', 'f', 'ccxts', 'ccxts/'),
('text3', 'f', 'ccxts', 'ccxts/'),
('text4', 'f', 'ccxts', 'ccxts/'),
('text5', 'f', 'ccxts', 'ccxts/'),
('text6', 'f', 'ccxts', 'ccxts/');

INSERT INTO GlobalFile VALUES
('JNBHJASDKNJNKAFD5S8ERAU7S8UJ18SJ', 'text1', '150465'),
('SHBGJASDKNJNKAFD5S8ERAU7S8UJ18SJ', 'text2', '150465'),
('QUAMJASDKNJNKAFD5S8ERAU7S8UJ18SJ', 'text3', '150465'),
('POLKJASDKNJNKAFD5S8ERAU7S8UJ18SJ', 'text4', '150465'),
('MNBVJASDKNJNKAFD5S8ERAU7S8UJ18SJ', 'text5', '150465'),
('VCGSJASDKNJNKAFD5S8ERAU7S8UJ18SJ', 'text6', '150465');
