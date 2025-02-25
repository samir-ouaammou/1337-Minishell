/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aahaded <aahaded@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 08:20:01 by aahaded           #+#    #+#             */
/*   Updated: 2025/02/17 08:20:04 by aahaded          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *get_path_env_utils(char **path, char *cmd)
{
    char *path_arg;
    char *temp;
    int i;

    i = 0;
    temp = NULL;
    while (path[i])
    {
        path_arg = ft_strjoin(path[i], "/");
        if (!path_arg)
            return (NULL);
        path_arg = ft_strjoin(path_arg, cmd);
        // free(path_arg);
        // if (!temp)
        //     return (NULL);
        // path_arg = temp;
        if (access(path_arg, X_OK) == 0)
            return (path_arg);
        i++;
    }
    if (temp)
        free(temp);
    return (NULL);
}


char *get_path_env(char *cmd, t_data *data)
{
    char **path;
    int i, check_path;
    char *path_arg;

    i = 0;
    check_path = 0;
    path = NULL;
    if (!data->env)
        return (NULL);
    while (data->env[i])
    {
        if (ft_strncmp(data->env[i], "PATH=", 5) == 0)
        {
            check_path = 1;
            path = ft_split(data->env[i] + 5, ':');
            break;
        }
        i++;
    }
    if (!check_path)
        return (NULL);
    path_arg = get_path_env_utils(path, cmd);
    free_all(path);
    if (!path_arg || cmd == NULL || is_operator(cmd))
    {
        if (path_arg)
            free(path_arg);
        return (NULL);
    }
    return (path_arg);
}