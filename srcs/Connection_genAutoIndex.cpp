/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection_genAutoIndex.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 21:36:38 by vcereced          #+#    #+#             */
/*   Updated: 2024/07/07 13:00:18 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Connection.hpp"

std::string genRowName(struct dirent *entry)
{
    std::string html;
    
    if (entry->d_type == DT_DIR)
    {
        std::cout << "adding this to html: " << "<a href=\"" << entry->d_name << "\\\">" << entry->d_name << "\\" << "</a>" << "\n";
        html = html + "<a href=\"" + entry->d_name + "\\\">" + entry->d_name + "\\" + "</a>";
    }
    else if (entry->d_type == DT_REG)
        html = html + "<a href=\"" + entry->d_name + "\">" + entry->d_name + "</a>";
    
    return html;
}


std::string genDescription(struct stat fileStat)
{
    std::string html;

    html += (fileStat.st_mode & S_IRUSR) ? "r" : "-";
    html += (fileStat.st_mode & S_IWUSR) ? "w" : "-";
    html += (fileStat.st_mode & S_IXUSR) ? "x" : "-";
    html += (fileStat.st_mode & S_IRGRP) ? "r" : "-";
    html += (fileStat.st_mode & S_IWGRP) ? "w" : "-";
    html += (fileStat.st_mode & S_IXGRP) ? "x" : "-";
    html += (fileStat.st_mode & S_IROTH) ? "r" : "-";
    html += (fileStat.st_mode & S_IWOTH) ? "w" : "-";
    html += (fileStat.st_mode & S_IXOTH) ? "x" : "-";

    return html;
}

std::string genRowLastMod(struct stat fileStat)
{
    std::string html;
    std::time_t modTime;
    struct tm   *timeinfo;
    char        buffer[100];

    modTime = fileStat.st_mtime;
    timeinfo = localtime(&modTime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    html = buffer;

    return html;
}

std::string genRowSize(struct stat fileStat)
{
    std::string          html;
    off_t                fileSize;
    std::ostringstream   oss;
    
    
    fileSize = fileStat.st_size;
    oss << fileSize;
    // html = std::to_string(fileSize) + " B";
    html = oss.str() + " B";

    return html;
}

std::string genRowAutoIndex(struct dirent *entry, std::string filePath)
{
    std::string html;
    struct stat fileStat;

    if (stat(filePath.c_str(), &fileStat) < 0)
        throw ServerException("genRowAutoIndex: " + std::string(strerror(errno)), INTERNAL_SERVER_ERROR);
      //  throw std::runtime_error(filePath);

    html += "<tr>";
    html += "<td>";
    html += genRowName(entry);
    html += "</td><td>";
    html += genRowLastMod(fileStat);
    html += "</td><td>";
    html += genRowSize(fileStat);
    html += "</td><td>";
    html += genDescription(fileStat);
    html += "</td>";
    html += "</tr>";

    return html;
}

std::string genRowsAutoIndex(std::string path)
{
    std::string     html;
    struct dirent   *entry;
    DIR             *dp = opendir(path.c_str());

    if (dp == NULL) {
        throw ServerException("genRowsAutoIndex: opendir: " + std::string(strerror(errno)), NOT_FOUND);
       // throw std::runtime_error("genRowsAutoIndex: opendir: " + std::string(strerror(errno)));
    }

    while ((entry = readdir(dp)))
    {
        try {
            html += genRowAutoIndex(entry, path + entry->d_name);
        } catch (const ServerException &e) {
            throw ServerException("genRowsAutoIndex: " + std::string(e.what()), e.getCode());
            //std::cerr << "genRowsAutoIndex: genRowAutoIndex: stats of file " << std::string(e.what()) << '\n';
        }
    }
    closedir(dp);

    return html;
}

// std::string Connection::genBodyAutoIndex(std::string path)
// {
//     std::string response_body_begin;
//     std::string response_body_middle;
//     std::string response_body_end;

//     response_body_begin = R"(
//     <!DOCTYPE html>
//     <html>
//     <head>
//         <title>Index of /directorio-listado/</title>
//         <style>
//             body {
//                 font-family: Arial, sans-serif;
//             }
//             table {
//                 width: 100%;
//                 border-collapse: collapse;
//             }
//             th, td {
//                 padding: 8px;
//                 text-align: left;
//                 border-bottom: 1px solid #ddd;
//             }
//             th {
//                 background-color: #f2f2f2;
//             }
//             a {
//                 color: #0066cc;
//                 text-decoration: none;
//             }
//             a:hover {
//                 text-decoration: underline;
//             }
//         </style>
//     </head>
//     <body>
//         <h1>Index of /directorio-listado/</h1>
//         <table>
//             <tr>
//                 <th>Name</th>
//                 <th>Last modified</th>
//                 <th>Size</th>
//                 <th>Permisions</th>
//             </tr>)";
//     response_body_middle = genRowsAutoIndex(path);
//     response_body_end = R"(
//             </table>
//         </body>
//     </html>)";

//     return response_body_begin + response_body_middle + response_body_end;
// }
std::string Connection::genBodyAutoIndex(std::string path)
{
    std::string response_body_begin;
    std::string response_body_middle;
    std::string response_body_end;

    response_body_begin =
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "    <title>Index of /directorio-listado/</title>\n"
        "    <style>\n"
        "        body {\n"
        "            font-family: Arial, sans-serif;\n"
        "        }\n"
        "        table {\n"
        "            width: 100%;\n"
        "            border-collapse: collapse;\n"
        "        }\n"
        "        th, td {\n"
        "            padding: 8px;\n"
        "            text-align: left;\n"
        "            border-bottom: 1px solid #ddd;\n"
        "        }\n"
        "        th {\n"
        "            background-color: #f2f2f2;\n"
        "        }\n"
        "        a {\n"
        "            color: #0066cc;\n"
        "            text-decoration: none;\n"
        "        }\n"
        "        a:hover {\n"
        "            text-decoration: underline;\n"
        "        }\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <h1>Index of /directorio-listado/</h1>\n"
        "    <table>\n"
        "        <tr>\n"
        "            <th>Name</th>\n"
        "            <th>Last modified</th>\n"
        "            <th>Size</th>\n"
        "            <th>Permisions</th>\n"
        "        </tr>";
    response_body_middle = genRowsAutoIndex(path);
    response_body_end =
        "        </table>\n"
        "    </body>\n"
        "</html>";

    return response_body_begin + response_body_middle + response_body_end;
}

