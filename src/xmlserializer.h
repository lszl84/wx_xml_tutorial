#pragma once

#include <wx/xml/xml.h>

#include "path.h"

struct XmlSerializer
{
    wxXmlDocument SerializePaths(const std::vector<Path> &paths)
    {
        wxXmlDocument doc;

        wxXmlNode *docNode = new wxXmlNode(wxXML_ELEMENT_NODE, "PaintDocument");
        docNode->AddAttribute("version", "1.1");

        for (const auto &path : paths)
        {
            wxXmlNode *pathNode = new wxXmlNode(wxXML_ELEMENT_NODE, "Object");
            pathNode->AddAttribute("type", "Path");
            pathNode->AddAttribute("color", path.color.GetAsString(wxC2S_HTML_SYNTAX));
            pathNode->AddAttribute("width", wxString::FromDouble(path.width));

            for (const auto &point : path.points)
            {
                wxXmlNode *pointNode = new wxXmlNode(wxXML_ELEMENT_NODE, "Point");
                pointNode->AddAttribute("x", wxString::FromDouble(point.m_x));
                pointNode->AddAttribute("y", wxString::FromDouble(point.m_y));
                pathNode->AddChild(pointNode);
            }

            docNode->AddChild(pathNode);
        }

        doc.SetRoot(docNode);

        return doc;
    }

    std::vector<Path> DeserializePaths(const wxXmlDocument &doc)
    {
        wxXmlNode *root = doc.GetRoot();

        std::vector<Path> squiggles;

        for (wxXmlNode *node = root->GetChildren(); node; node = node->GetNext())
        {
            if (node->GetName() != "Object")
                continue;

            if (node->GetAttribute("type") == "Path")
            {
                Path squiggle;
                squiggle.color = wxColor(node->GetAttribute("color"));
                squiggle.width = wxAtof(node->GetAttribute("width"));

                for (wxXmlNode *pointNode = node->GetChildren(); pointNode; pointNode = pointNode->GetNext())
                {
                    if (pointNode->GetName() != "Point")
                        continue;

                    squiggle.points.push_back(wxPoint2DDouble(wxAtof(pointNode->GetAttribute("x")),
                                                              wxAtof(pointNode->GetAttribute("y"))));
                }

                squiggles.push_back(squiggle);
            }
        }

        return squiggles;
    }
};