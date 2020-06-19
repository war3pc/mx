use super::musicxml_xsd::{Enumeration, MusicXSD, SimpleType, TypeDefinition};

use crate::generate::musicxml_xsd_parser::Error::SchemaNotFound;
use derive_more::Display;
use exile::{Document, Element};
use std::collections::HashMap;
use std::fmt::{Display, Formatter};
use std::sync::atomic::{AtomicUsize, Ordering};

#[derive(Debug, Display, Clone, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub(crate) enum Error {
    SchemaNotFound,
    UnknownSchemaNode(String),
    NameAttributeNotFound(String),
    XlinkNamespaceNotFound,
    DuplicateXsInfoID(String),
    UnexpectedNode(String),
    SimpleTypeBaseNotFound,
    ExpectedOneChildOfXsAnnotation,
    ExpectedDocumentationNode,
    ExpectedDocumentationNodeToReturnText,
}

static COUNTER: AtomicUsize = AtomicUsize::new(0);

pub(crate) fn parse_musicxml_xsd(doc: &Document) -> Result<MusicXSD, Error> {
    let root = doc.root();
    if root.fullname().as_str() != "xs:schema" {
        return Err(SchemaNotFound);
    }
    let mut type_nodes = parse_type_nodes(doc)?;
    let type_definitions = parse_type_definitions(&type_nodes)?;
    for (_, n) in type_nodes {
        println!("{}", n);
    }

    Ok(MusicXSD::new(type_definitions))
}

fn parse_type_nodes(doc: &Document) -> Result<HashMap<String, XsTypeNode>, Error> {
    let mut type_nodes = HashMap::new();
    for node in doc.root().children() {
        let x = parse_xs_info(node)?;
        if let Some(existing) = type_nodes.insert(x.id.clone(), x) {
            return Err(Error::DuplicateXsInfoID(existing.id.clone()));
        }
    }
    Ok(type_nodes)
}

const XS_ANNOTATION: &str = "xs:annotation";
const XS_ATTRIBUTE_GROUP: &str = "xs:attributeGroup";
const XS_COMPLEX_TYPE: &str = "xs:complexType";
const XS_ELEMENT: &str = "xs:element";
const XS_GROUP: &str = "xs:group";
const XS_IMPORT: &str = "xs:import";
const XS_SIMPLE_TYPE: &str = "xs:simpleType";

#[derive(Debug, Clone)]
pub(crate) struct XsTypeNode<'a> {
    pub(crate) xsd_type: XsType,
    pub(crate) index: usize,
    pub(crate) id: String,
    pub(crate) name: String,
    pub(crate) node: &'a Element,
}

impl<'a> Display for XsTypeNode<'_> {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}-{}-{}", self.index, self.id, self.name)
    }
}

fn parse_xs_info(node: &Element) -> Result<XsTypeNode, Error> {
    let index = COUNTER.fetch_add(1, Ordering::Relaxed);
    let xsd_type = XsType::parse(node)?;
    let name = match xsd_type {
        XsType::XsAnnotation => format!("{}:{}", XS_ANNOTATION, index),
        XsType::XsImport => format!(
            "{}:{}",
            XS_IMPORT,
            node.attributes
                .map()
                .get("namespace")
                .ok_or_else(|| Error::NameAttributeNotFound(node.fullname()))?
        ),
        _ => node
            .attributes
            .map()
            .get("name")
            .ok_or_else(|| Error::XlinkNamespaceNotFound)?
            .clone(),
    };
    let id = match xsd_type {
        XsType::XsAnnotation | XsType::XsImport => name.clone(),
        _ => format!("{}:{}", node.fullname(), name),
    };
    Ok(XsTypeNode {
        xsd_type,
        index,
        id,
        name,
        node,
    })
}

#[derive(Debug, Display, Clone, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub(crate) enum XsType {
    XsAnnotation,
    XsAttributeGroup,
    XsComplexType,
    XsElement,
    XsGroup,
    XsImport,
    XsSimpleType,
}

impl XsType {
    fn parse(node: &Element) -> Result<Self, Error> {
        let fullname = node.fullname();
        match fullname.as_str() {
            XS_ANNOTATION => Ok(XsType::XsAnnotation),
            XS_ATTRIBUTE_GROUP => Ok(XsType::XsAttributeGroup),
            XS_COMPLEX_TYPE => Ok(XsType::XsComplexType),
            XS_ELEMENT => Ok(XsType::XsElement),
            XS_GROUP => Ok(XsType::XsGroup),
            XS_IMPORT => Ok(XsType::XsImport),
            XS_SIMPLE_TYPE => Ok(XsType::XsSimpleType),
            _ => Err(Error::UnknownSchemaNode(fullname)),
        }
    }
}

fn parse_type_definitions(
    xsd_nodes: &HashMap<String, XsTypeNode>,
) -> Result<Vec<TypeDefinition>, Error> {
    let mut type_definitions = Vec::new();
    for (_, x) in xsd_nodes {
        if (x.xsd_type != XsType::XsSimpleType) {
            continue;
        }
        if is_enumeration_simple_type(x) {
            let eee = parse_enumeration(x)?;
            type_definitions.push(TypeDefinition::Simple(SimpleType::Enum(eee)));
        }
    }
    Ok(type_definitions)
}

fn is_enumeration_simple_type(node: &XsTypeNode) -> bool {
    for child in node.node.children() {
        if child.fullname() == "xs:restriction" {
            for restriction in child.children() {
                if restriction.fullname() == "xs:enumeration" {
                    return true;
                }
            }
        }
    }
    false
}

fn parse_enumeration(node: &XsTypeNode) -> Result<Enumeration, Error> {
    let mut en = Enumeration::default();
    en.id = node.id.clone();
    en.name = node.name.clone();
    en.index = node.index;
    for child in node.node.children() {
        if child.fullname() == "xs:restriction" {
            if let Some(base) = child.attributes.map().get("base") {
                en.base = base.clone();
            } else {
                return Err(Error::SimpleTypeBaseNotFound);
            }
            for restriction in child.children() {
                if restriction.fullname() == "xs:enumeration" {
                    if let Some(value) = restriction.attributes.map().get("value") {
                        en.members.push(value.clone())
                    }
                } else {
                    return Err(Error::UnexpectedNode(restriction.fullname()));
                }
            }
        } else if child.fullname().as_str() == XS_ANNOTATION {
            let mut docsnodes = child.children();
            let documentation_node = docsnodes
                .next()
                .ok_or_else(|| Error::ExpectedOneChildOfXsAnnotation)?;
            if documentation_node.fullname().as_str() != "xs:documentation" {
                return Err(Error::ExpectedDocumentationNode);
            }
            en.documentation = documentation_node
                .text()
                .ok_or_else(|| Error::ExpectedDocumentationNodeToReturnText)?;
        }
    }
    Ok(en)
}