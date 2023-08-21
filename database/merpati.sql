-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Aug 22, 2023 at 01:13 AM
-- Server version: 10.4.22-MariaDB
-- PHP Version: 7.4.27

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `merpati`
--

-- --------------------------------------------------------

--
-- Table structure for table `burung`
--

CREATE TABLE `burung` (
  `id_burung` int(11) NOT NULL,
  `nama_burung` varchar(20) NOT NULL,
  `rfid` varchar(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `burung`
--

INSERT INTO `burung` (`id_burung`, `nama_burung`, `rfid`) VALUES
(1, 'Ardian', '3067700'),
(2, 'Yoga', '3102713'),
(3, 'Jono', '3085881'),
(4, 'Upin', '3090706');

-- --------------------------------------------------------

--
-- Table structure for table `datang`
--

CREATE TABLE `datang` (
  `id_datang` int(11) NOT NULL,
  `id_matchup` int(11) NOT NULL,
  `id_burung` int(11) NOT NULL,
  `waktu_datang` datetime NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `datang`
--

INSERT INTO `datang` (`id_datang`, `id_matchup`, `id_burung`, `waktu_datang`) VALUES
(1, 1, 1, '2023-08-22 06:10:49'),
(2, 1, 2, '2023-08-22 06:11:16'),
(3, 1, 3, '0000-00-00 00:00:00'),
(4, 1, 4, '0000-00-00 00:00:00');

-- --------------------------------------------------------

--
-- Table structure for table `matchup`
--

CREATE TABLE `matchup` (
  `id_matchup` int(11) NOT NULL,
  `id_pertandingan` int(11) NOT NULL,
  `id_track` int(11) NOT NULL,
  `waktu_matchup` datetime NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  `status` tinyint(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `matchup`
--

INSERT INTO `matchup` (`id_matchup`, `id_pertandingan`, `id_track`, `waktu_matchup`, `status`) VALUES
(1, 1, 1, '2023-08-22 06:09:14', 1);

-- --------------------------------------------------------

--
-- Table structure for table `pertandingan`
--

CREATE TABLE `pertandingan` (
  `id_pertandingan` int(11) NOT NULL,
  `nama_pertandingan` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `pertandingan`
--

INSERT INTO `pertandingan` (`id_pertandingan`, `nama_pertandingan`) VALUES
(1, 'Magetan Cup');

-- --------------------------------------------------------

--
-- Table structure for table `track`
--

CREATE TABLE `track` (
  `id_track` int(11) NOT NULL,
  `nama_track` varchar(20) NOT NULL,
  `start_track` varchar(30) NOT NULL,
  `finish_track` varchar(30) NOT NULL,
  `panjang_track` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `track`
--

INSERT INTO `track` (`id_track`, `nama_track`, `start_track`, `finish_track`, `panjang_track`) VALUES
(1, 'GorgaMagetan', '-7.6820976,111.4170272', '-7.6556506,111.3264188', 12000);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `burung`
--
ALTER TABLE `burung`
  ADD PRIMARY KEY (`id_burung`);

--
-- Indexes for table `datang`
--
ALTER TABLE `datang`
  ADD PRIMARY KEY (`id_datang`);

--
-- Indexes for table `matchup`
--
ALTER TABLE `matchup`
  ADD PRIMARY KEY (`id_matchup`);

--
-- Indexes for table `pertandingan`
--
ALTER TABLE `pertandingan`
  ADD PRIMARY KEY (`id_pertandingan`);

--
-- Indexes for table `track`
--
ALTER TABLE `track`
  ADD PRIMARY KEY (`id_track`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `burung`
--
ALTER TABLE `burung`
  MODIFY `id_burung` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT for table `datang`
--
ALTER TABLE `datang`
  MODIFY `id_datang` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT for table `matchup`
--
ALTER TABLE `matchup`
  MODIFY `id_matchup` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `pertandingan`
--
ALTER TABLE `pertandingan`
  MODIFY `id_pertandingan` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `track`
--
ALTER TABLE `track`
  MODIFY `id_track` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
