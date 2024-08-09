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
	file_hash VARCHAR(64) NOT NULL,
	filepath VARCHAR(128) NOT NULL,
	CONSTRAINT FOREIGN KEY (username) REFERENCES User(username) ON DELETE CASCADE
);

DROP TABLE IF EXISTS GlobalFile;
CREATE TABLE GlobalFile
(
	file_hash VARCHAR(64) PRIMARY KEY,
	filename VARCHAR(255) NOT NULL,

	filesize BIGINT NOT NULL,
	usercount INT DEFAULT 1

);

UPDATE User set pwd = '/ccxts&' where username = 'ccxts';

INSERT INTO User VALUES
('ccxts', 'JNBHJASDKNJNKAFD5S8ERAU7S8UJ18SJ', NULL);

INSERT INTO File(filename, filetype, username, file_hash, filepath) VALUES
('text1', 'f', 'ccxts', 'JNBHJASDKNJNKAFD5S8ERAU7S8UJ18SJ', '/ccxts'),
('text2', 'f', 'ccxts', 'SHBGJASDKNJNKAFD5S8ERAU7S8UJ18SJ', '/ccxts'),
('text3', 'f', 'ccxts', 'QUAMJASDKNJNKAFD5S8ERAU7S8UJ18SJ', '/ccxts'),
('text4', 'f', 'ccxts', 'POLKJASDKNJNKAFD5S8ERAU7S8UJ18SJ', '/ccxts'),
('text5', 'f', 'ccxts', 'MNBVJASDKNJNKAFD5S8ERAU7S8UJ18SJ', '/ccxts'),
('text6', 'f', 'ccxts', 'VCGSJASDKNJNKAFD5S8ERAU7S8UJ18SJ', '/ccxts');

INSERT INTO GlobalFile(file_hash, filename, filesize) VALUES
('JNBHJASDKNJNKAFD5S8ERAU7S8UJ18SJ', 'text1', '150465'),
('SHBGJASDKNJNKAFD5S8ERAU7S8UJ18SJ', 'text2', '15852565'),
('QUAMJASDKNJNKAFD5S8ERAU7S8UJ18SJ', 'text3', '780465'),
('POLKJASDKNJNKAFD5S8ERAU7S8UJ18SJ', 'text4', '54865'),
('MNBVJASDKNJNKAFD5S8ERAU7S8UJ18SJ', 'text5', '425478'),
('VCGSJASDKNJNKAFD5S8ERAU7S8UJ18SJ', 'text6', '1788465');
