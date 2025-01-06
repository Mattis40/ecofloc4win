/**
 * @file gpu.cpp
 * @brief Récupérer les metrics du GPU
 * @author Mattis PINGARD
 * @date 2025-01-06
*/

#pragma once

#include <vector>

/**
 * @namespace GPU
 * @brief Donne accès à des fonctions permettant de récupérer les metrics du GPU
 */
namespace GPU
{
	/**
     * @brief Renvoie le pourcentage d'utilisation d'une liste de processus
     *
     * Cette fonction renvoie à partir d'une liste d'id de processus le pourcentage d'utilisation
     * du GPU de ces derniers
	 *
     * @param pids La liste des pids des processus ciblés par la fonction
     * @return Retourne une liste contenant le pourcentage d'utilisation du processus ou -1 pour chaque pid en cas d'erreur
    */
	std::vector<int> getGPUUsage(std::vector<int> pids);

	/**
     * @brief Récupère la puissance actuelle du GPU
     *
     * Cette fonction permet de récupérer la puissance consommé par le GPU à l'instant t
     * elle retourne donc un entier correspondant à la puissance en Watt (W)
	 *
     * @return Retourne un entier correspondant à la puissance en Watt (W) ou -1 si une erreur est survenue
    */
	int getGPUPower();

	/**
     * @brief Récupère l'energie consommer par une liste de processus
     *
     * Cette fonction permet de récupérer l'énergie consommée par des processus spécifiés,
     * elle retourne une liste contenant l'énergie consommée en Joules (J)
	 *
     * @return Retourne une liste contenant l'énergie consommé en Joules (J) ou -1 si une erreur est survenue
    */
	int getGPUJoules(std::vector<int> pids, int ms);
};
