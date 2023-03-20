CREATE DATABASE `makerspace`;
use `makerspace`;

CREATE TABLE `id_uuid`
(
	`id` VARCHAR(64) NOT NULL PRIMARY KEY,
	`uuid` UUID NOT NULL
);

CREATE TABLE `user_data`
(
	`uuid` UUID NOT NULL PRIMARY KEY,
	`email` VARCHAR(256) NOT NULL UNIQUE,
	`type` enum('student', 'faculty', 'staff', 'other') NOT NULL DEFAULT 'student',
	`name` VARCHAR(256) NOT NULL,
	`last_scan` date,
	`creation` date NOT NULL DEFAULT CURRENT_DATE
);

CREATE TABLE `laser`
(
	`uuid` UUID NOT NULL PRIMARY KEY,
	`training` enum('untrained', 'partial', 'fully', 'canTrain') NOT NULL DEFAULT 'untrained',
	`training_date` date NOT NULL DEFAULT CURRENT_DATE,
	`rotary` boolean NOT NULL DEFAULT false,
	`small_laser` boolean NOT NULL DEFAULT false,
	FOREIGN KEY(uuid) REFERENCES user_data(uuid) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE `3d_printers`
(
	`uuid` UUID NOT NULL PRIMARY KEY,
	`training` enum('untrained', 'partial', 'fully', 'canTrain') NOT NULL DEFAULT 'untrained',
	`training_date` date NOT NULL DEFAULT CURRENT_DATE,
	`print_starting` boolean NOT NULL DEFAULT false,
	`ultimaker` boolean NOT NULL DEFAULT false,
	FOREIGN KEY(uuid) REFERENCES user_data(uuid) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE `hand_tools`
(
	`uuid` UUID NOT NULL PRIMARY KEY,
	`training` enum('untrained', 'partial', 'fully', 'canTrain') NOT NULL DEFAULT 'untrained',
	`training_date` date NOT NULL DEFAULT CURRENT_DATE,
	FOREIGN KEY(uuid) REFERENCES user_data(uuid) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE `woodshop`
(
	`uuid` UUID NOT NULL PRIMARY KEY,
	`training` enum('untrained', 'partial', 'fully', 'canTrain') NOT NULL DEFAULT 'untrained',
	`training_date` date NOT NULL DEFAULT CURRENT_DATE,
	`waiver` boolean NOT NULL,
	FOREIGN KEY(uuid) REFERENCES user_data(uuid) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE `embroidery`
(
	`uuid` UUID NOT NULL PRIMARY KEY,
	`training` enum('untrained', 'partial', 'fully', 'canTrain') NOT NULL DEFAULT 'untrained',
	`training_date` date NOT NULL DEFAULT CURRENT_DATE,
	`cap` boolean NOT NULL DEFAULT false,

	FOREIGN KEY(uuid) REFERENCES user_data(uuid) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE `shopbot`
(
	`uuid` UUID NOT NULL PRIMARY KEY,
	`training` enum('untrained', 'partial', 'fully', 'canTrain') NOT NULL DEFAULT 'untrained',
	`training_date` date NOT NULL DEFAULT CURRENT_DATE,
	`rotary` boolean NOT NULL DEFAULT false,
	FOREIGN KEY(uuid) REFERENCES user_data(uuid) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE `vinyl`
(
	`uuid` UUID NOT NULL PRIMARY KEY,
	`training` enum('untrained', 'partial', 'fully', 'canTrain') NOT NULL DEFAULT 'untrained',
	`training_date` date NOT NULL DEFAULT CURRENT_DATE,
	`roland` boolean NOT NULL DEFAULT true,
	`cricut` boolean NOT NULL DEFAULT false,
	`graphgear` boolean NOT NULL DEFAULT false,
	`heat_press` boolean NOT NULL DEFAULT false,
	FOREIGN KEY(uuid) REFERENCES user_data(uuid) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE `spray_booth`
(
	`uuid` UUID NOT NULL PRIMARY KEY,
	`training` enum('untrained', 'partial', 'fully', 'canTrain') NOT NULL DEFAULT 'untrained',
	`training_date` date NOT NULL DEFAULT CURRENT_DATE,
	`paint_sprayer` boolean NOT NULL,

	FOREIGN KEY(uuid) REFERENCES user_data(uuid) ON UPDATE CASCADE ON DELETE CASCADE
);

-- Template Table
/* 
CREATE TABLE `<MACHINE>`
(
	`uuid` UUID NOT NULL PRIMARY KEY,
	`training` enum('untrained', 'partial', 'fully', 'canTrain') NOT NULL DEFAULT 'untrained',
	`training_date` date NOT NULL DEFAULT CURRENT_DATE,
	

	FOREIGN KEY(uuid) REFERENCES user_data(uuid) ON UPDATE CASCADE ON DELETE CASCADE
);
*/
