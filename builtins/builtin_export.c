/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: souaammo <souaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 18:11:56 by aahaded           #+#    #+#             */
/*   Updated: 2025/03/03 12:19:54 by souaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void print_export(t_exaction *data)
{
	(void)data;
	int i;

	if (!data || !data_struc()->export)
		return;
	i = 0;
	while (data_struc()->export[i])
	{
		if (data_struc()->export[i])
			printf("declare -x %s\n", data_struc()->export[i]);
		i++;
	}
}

static int update_env_var(char **args, t_exaction *data, char *str, int i)
{
	(void)data;
	int j;
	int found;
	char *str_j;

	j = 0;
	found = 0;
	str_j = NULL;
	if (!data_struc()->env)
		return (0);
	while (data_struc()->env[j])
	{
		if (ft_strncmp(data_struc()->env[j], args[i], (ft_strlen(args[i]) - ft_strlen(str))) == 0)
		{
			if (data_struc()->is_plus == 1)
			{
				str_j = ft_strjoin(data_struc()->env[j], str + 1);
				if (!str_j)
					return (0);
				data_struc()->env[j] = str_j;
			}
			else
			{
				data_struc()->env[j] = ft_strdup(args[i]);
				if (!data_struc()->env[j])
					return (0);
			}
			found = 1;
			break;
		}
		j++;
	}
	return (found);
}

char *get_key_part(char *str, char delimiter)
{
	char *result;
	char *pos;
	int len;

	pos = ft_strchr(str, delimiter);
	if (pos)
	{
		len = (ft_strlen(str) - ft_strlen(pos)) + 1;
		result = ft_malloc(len + 1);
		if (!result)
			return (NULL);
		ft_strncpy(result, str, len);
		result[len] = '\0';
		return (result);
	}
	return (NULL);
}

static int update_export_var(char **args, t_exaction *data, char *str, int i)
{
	(void)data;
	int j;
	int found;
	char *str_new;

	if (!data_struc()->export)
		return (0);
	j = 0;
	found = 0;
	while (data_struc()->export[j])
	{
		if (ft_strncmp(data_struc()->export[j], args[i], (ft_strlen(args[i]) - ft_strlen(str))) == 0)
		{
			if (data_struc()->is_plus == 1)
			{
				char *sss = add_double_quotes_plus((ft_strchr(data_struc()->export[j], '=') + 1), str + 1);
				char *get_key = get_key_part(args[i], '=');
				char *str_j = ft_strjoin(get_key, sss);
				char *ddd = add_double_quotes(str_j);
				data_struc()->export[j] = ddd;
			}
			else
			{
				str_new = add_double_quotes(args[i]);
				data_struc()->export[j] = ft_strdup(str_new);
				if (!data_struc()->export[j])
					return (0);
			}
			found = 1;
			break;
		}
		j++;
	}
	return (found);
}

static int add_default_export(char *str, t_exaction *data, int len)
{
	(void)data;
	(void)str;
	char **new_export;
	int count;
	int i;

	count = 0;
	i = 0;
	new_export = NULL;
	while (data_struc()->export[i])
	{
		if (ft_strncmp(data_struc()->export[i], str, len) == 0)
			return (1);
		i++;
	}
	i = 0;
	if (data_struc()->export)
	{
		while (data_struc()->export[count])
			count++;
	}
	new_export = ft_malloc(sizeof(char *) * (count + 2));
	if (!new_export)
		return (1);
	while (i < count)
	{
		new_export[i] = ft_strdup(data_struc()->export[i]);
		i++;
	}
	new_export[i] = ft_strdup(str);
	new_export[i + 1] = NULL;
	data_struc()->export = new_export;
	return (0);
}

static int add_new_env_var(char **args, t_exaction *data, int index)
{
	(void)data;
	char **new_env;
	int count;
	int i;

	count = 0;
	i = 0;

	if (data_struc()->env)
	{
		while (data_struc()->env[count])
			count++;
	}
	new_env = ft_malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	while (i < count)
	{
		new_env[i] = ft_strdup(data_struc()->env[i]);
		i++;
	}
	new_env[i] = ft_strdup(args[index]);
	new_env[i + 1] = NULL;
	data_struc()->env = new_env;
	return (0);
}

static int add_new_export_var(char **args, t_exaction *data, int index)
{
	(void)data;
	char **new_export;
	int count;
	int i;

	count = 0;
	i = 0;
	if (data_struc()->export)
	{
		while (data_struc()->export[count])
			count++;
	}
	new_export = ft_malloc(sizeof(char *) * (count + 2));
	if (!new_export)
		return (1);
	while (i < count)
	{
		new_export[i] = ft_strdup(data_struc()->export[i]);
		i++;
	}
	new_export[i] = ft_strdup(args[index]);
	new_export[i + 1] = NULL;
	data_struc()->export = new_export;
	return (0);
}

int check_op(char *str)
{
	int i = 0;
	while (str[i])
	{
		if (str[0] == '-' || str[0] == '+' || str[0] == '=' || ft_isdigit(str[0]) || !ft_isalpha(str[0]))
			return (1);
		else if ((!ft_isalpha(str[i]) && !ft_isdigit(str[i]) && str[i] != '+'))
			return (1);
		i++;
	}
	return (0);
}

int check_plus(char *str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			break;
		if (str[i] == '+' && (str[i + 1] != '='))
			return (1);
		i++;
	}
	return (0);
}

char *add_double_quotes_plus(char *str_export, char *str)
{
    int i;
    int j;
    int len;
    char *res;

    if (!str_export || !str)
        return (NULL);
    i = 0;
    len = 0;
    while (str_export[i])
    {
        if (str_export[i] != '"')
            len++;
        i++;
    }
    res = ft_malloc(len + ft_strlen(str) + 1);
    if (!res)
        return (NULL);
    i = 0;
    j = 0;
    while (str_export[i])
    {
        if (str_export[i] != '"')
        {
            res[j] = str_export[i];
            j++;
        }
        i++;
    }
    i = 0;
    while (str[i])
    {
        res[j + i] = str[i];
        i++;
    }
    res[j + i] = '\0';
    return (res);
}

char *add_double_quotes(char *str)
{
	int i;
	int j;
	int len;
	char *res;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	res = ft_malloc(len + 3);
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '=')
		{
			res[j++] = str[i++];
			res[j++] = '"';
		}
		else
			res[j++] = str[i++];
	}
	res[j++] = '"';
	res[j] = '\0';
	return (res);
}

int builtin_export(char **args, t_exaction *data)
{
	(void)args;
	int i;
	int found;
	int found_export;
	char *str;

	if (!args || !data || !data_struc()->env || !data_struc()->export)
		return (1);
	i = 1;
	found = 0;
	found_export = 0;
	data_struc()->is_plus = 0;
	str = NULL;

	if (!args[1])
		print_export(data);
	else
	{
		while (args[i])
		{
			int j = 0;
			int check_error = 0;
			char **split_str = ft_split(args[i], '=');
			int index_check = check_plus(args[i]);
			if (args[i][0] == '=' || index_check || check_op(split_str[0]) == 1)
			{
				ft_printf("minishell: export: `%s': not a valid identifier\n", args[i]);
				check_error = 1;
				data_struc()->exit_status = 1;
				free_all(split_str);
			}
			str = ft_strchr(args[i], '=');
			if (check_error || (args[i][0] == '_' && args[i][1] == '='))
			{
				i++;
				continue;
			}
			free_all(split_str);
			if (str)
			{
				j = 0;
				while (args[i][j])
				{
					if (args[i][j] == '+')
						data_struc()->is_plus = 1;
					if (args[i][j] == '=')
						break;
					j++;
				}
				if (data_struc()->is_plus == 1)
				{
					char *res;
					int len;
					int j;
					int k;

					len = 0;
					j = 0;
					while (args[i][j])
					{
						if (args[i][j] != '+')
							len++;
						j++;
					}
					res = ft_malloc(len + 1);
					if (!res)
						return (1);
					j = 0;
					k = 0;
					while (args[i][j])
					{
						if (args[i][j] != '+')
							res[k++] = args[i][j];
						j++;
					}
					res[k] = '\0';
					args[i] = ft_strdup(res);
				}
				found = update_env_var(args, data, str, i);
				found_export = update_export_var(args, data, str, i);
				if (!found)
					add_new_env_var(args, data, i);
				if (!found_export)
				{
					args[i] = add_double_quotes(args[i]);
					add_new_export_var(args, data, i);
				}
			}
			else
				add_default_export(args[i], data, (ft_strlen(args[i]) - ft_strlen(str)));
			i++;
		}
	}
	return (0);
}
